//! \file      transaction.h
//! \brief     Transaction class definition

#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "depresolver.h"
#include "locker.h"
#include "process.h"
#include "repository.h"

using namespace std;

class ArgParser;
class Config;
class Locker;
class Package;
class PkgDB;
class Repository;

//! \class  Transaction
//! \brief  To install, update, remove a package or a list of packages,
//!         and to calculate their dependencies.
class Transaction
{
public:
  //! Transaction type
  enum Transaction_t
  {
    NONE,     //!<  None transaction of the following:
    DEPCALC,  //!<  Calculate the package(s) dependencies
    INSTALL,  //!<  Install the package(s)
    UPDATE,   //!<  Update the package(s)
    REMOVE    //!<  Remove the package(s)
  };

  //! Result of transaction execution
  enum Result_t
  {
    // Generic

    SUCCESS           =  0, //!<  Success

    // Related to transaction arguments

    NO_PACKAGE_GIVEN  = -1, //!< No package(s) given to the transaction
    PACKAGE_NOT_FOUND = -2, //!< Given package not found in the repo

    // Related to dependencies calculation

    CYCLIC_DEPEND     = -3, //!< Cyclic dependencies found

    // Related to log file creation for logging the
    // INSTALL/UPDATE transaction

    NO_LOG_FILE       = -4, //!< No log file specified
    LOG_DIR_FAILURE   = -5, //!< Couldn't create the log directory
    LOG_FILE_FAILURE  = -6, //!< Couldn't create the log file
    LOG_FILE_EEXIST   = -7, //!< The log file already exists

    // Related to package removing

    PKGRM_E_GENERAL   = -8, //!< Remove error

    // Related to package building/installing ( see man pkgmk(8) )

    PKGMK_E_GENERAL   =  1, //!< General error
    PKGMK_E_PKGFILE   =  2, //!< Invalid Pkgfile

    PKGMK_E_DIR_PERM  =  3, //!< Source/build directory missing or
                            //!< missing read/write permission

    PKGMK_E_DOWNLOAD  =  4, //!< Error during download
    PKGMK_E_UNPACK    =  5, //!< Error during unpacking of source files
    PKGMK_E_MD5       =  6, //!< md5sum verification failed
    PKGMK_E_FOOTPRINT =  7, //!< Footprint check failure
    PKGMK_E_BUILD     =  8, //!< Error while running 'build()'

    PKGMK_E_INSTALL   =  9, //!< Error while executing 'addcommand'
  };

  //! \struct  pkgRunScriptsState_t
  //! \brief   To save the results of pre/post-... scripts
  //!          for a package in the current transaction.
  //!
  //! \note    In the case of INSTALL transaction type, to save the
  //!          results of the pre/post-install scripts, in case of
  //!          REMOVE -- pre/post-remove scripts.
  struct pkgRunScriptsState_t
  {
    // -1 by default means the package has no pre/post-... scripts
    int pre  = -1;
    int post = -1;

    //! \brief   Get the package's pre/post-... scripts execution state
    //!
    //! \return  the string with package's pre/post-... scripts
    //!          execution state
    string strRunScriptsState() const
    {
      string state = "";
      if ( pre  != -1 )
        state += pre  == 0 ? "[pre: ok] " : "[pre: fail] ";
      if ( post != -1 )
        state += post == 0 ? "[post: ok]" : "[post: fail]";
      return state;
    }
  };

  //! \brief   Construct a Transaction object
  //!
  //! \tparam  names   the package source name or a list of packages
  //!                  sources names to be processed
  //! \tparam  repo    the repository to look for packages
  //! \tparam  pkgDB   the package database of installed packages
  //! \tparam  parser  the argument parser
  //! \tparam  config  the configuration file parser
  //! \tparam  locker  the package locker
  template< typename T >
    Transaction( const T&           names,
                 const Repository*  repo,
                 PkgDB*             pkgDB,
                 const ArgParser*   parser,
                 const Config*      config,
                 Locker*            locker )
      : m_repo( repo ),
        m_pkgDB( pkgDB ),
        m_parser( parser ),
        m_config( config ),
        m_locker( locker ),
        m_resolver(),
        m_transactionType( NONE ),
        m_logfd( -1 ),
        m_depCalced( false ),
        m_depNameList(),
        m_depList(),
        m_installedPackages(),
        m_removedPackages(),
        m_alreadyInstalledPackages(),
        m_ignoredPackages(),
        m_missingPackages(),
        m_downloadFailedPackages(),
        m_buildFailedPackages(),
        m_installFailedPackages(),
        m_removeFailedPackages()
    {
      if constexpr ( is_same< T, pkgname_t >::value )
      {
        // Got 'string' as argument
        m_packages.push_back(
            make_pair( names, m_repo->getPackage( names )  ) );
      }
      else if constexpr
        ( is_same< T, map< pkgname_t, pkgver_t > >::value )
      {
        // Got 'map< name, version >' as argument
        for ( const auto& [ name, version ]: names )
          m_packages.push_back(
              make_pair( name, m_repo->getPackage( name ) ) );
      }
      else if constexpr
        ( is_same< T, map< pkgname_t, Package* > >::value )
      {
        // Got 'map< name, Package* >' as argument
        for ( const auto& [ name, pkg ]: names )
          m_packages.push_back( make_pair( name, pkg ) );
      }
      else
      {
        // Got 'list< T >' as argument
        for ( const auto& name: names )
          m_packages.push_back(
              make_pair( name, m_repo->getPackage( name ) ) );
      }
    }

  //! \brief   Get the worked transaction type
  //!
  //! \return  a \a Transaction_t value indicating which type of
  //!          transaction worked out
  const Transaction_t& type() const;

  //! \brief   Get the transaction execution result
  //!
  //! \return  a \a Result_t value indicating the transaction
  //!          execution result
  const Result_t& result() const;

  //! \brief   Install/Update the packages indicated in the current
  //!          transaction
  //!
  //! \param   transactionType  INSTALL or UPDATE the current
  //!                           transaction packages
  //!
  //! \return  a \a Result_t value indicating the transaction
  //!          execution result
  //!
  //! \note    Also sets the class member \a m_transactionResult value
  //!          to this result.
  const Result_t& install( Transaction_t transactionType );

  //! \brief   Remove the packages indicated in the current transaction
  //!
  //! \return  a \a Result_t value indicating the transaction execution
  //!          result
  const Result_t& remove();

  //! \brief   Calculate dependendencies for the packages indicated
  //!          in the current transaction
  //!
  //! \return  a \a Result_t value indicating the transaction
  //!          execution result
  const Result_t& calcDeps();

  //! \brief   Get the list of packages required for the install
  //!          transaction
  //!
  //! The list of packages that should be installed to meet the
  //! requirements for the packages to be installed.
  //! Includes the packages to be installed.
  //!
  //! \note    The list is formed as a result of \a calcDeps() call.
  //!          The packages are in correct order: packages to be
  //!          installed first come first.
  //!
  //! \return  the list of packages required for the install transaction
  const list< pkgname_t >& deps() const;

  //! \brief   Get the packages that were installed in this transaction
  //!
  //! \return  the list or pairs, where
  //!          - pair.first   is the package name
  //!          - pair.second  is the package pre/post-install scripts
  //!                         execution info
  const list< pair< pkgname_t, pkgRunScriptsState_t > >& installed()
    const;

  //! \brief   Get the packages that were removed in this transaction
  //!
  //! \return  the list of pairs, where
  //!          - pair.first   is the package name
  //!          - pair.second  is the package pre/post-remove scripts
  //!                         execution info
  const list< pair< pkgname_t, pkgRunScriptsState_t > >& removed()
    const;

  //! \brief   Get the packages that were requested to be installed
  //!          but are already installed
  //!
  //! \return  the list of already installed packages
  const list< pkgname_t >& alreadyInstalled() const;

  //! \brief   Get the packages that were ignored in this transaction,
  //!          or selected to be ignored by the user through
  //!          command-line
  //!
  //! \return  the list of ignored packages
  const list< pkgname_t >& ignored() const;

  //! \brief   Get the list of packages that cannot be installed
  //!          because they could not be found in the packages sources
  //!          tree
  //!
  //! \return  the list of pairs, where
  //!          - pair.first   is the package missing in the packages
  //!                         sources tree
  //!          - pair.second  is the package requiring \a pair.first
  const list< pair< pkgname_t, pkgname_t > >& missing() const;

  //! \brief   Get the list of packages whose source(s) could not be
  //!          downloaded
  //!
  //! \return  the list of package whose source(s) downloading failed
  const list< pkgname_t >& downloadFailed() const;

  //! \brief   Get the list of packages that were building failed
  //!
  //! \return  the list of packages that were building failed
  const list< pkgname_t >& buildFailed() const;

  //! \brief   Get the list of packages that were installation/update
  //!          failed
  //!
  //! \return  the list of pairs, where
  //!          - pair.first   is the package name
  //!          - pair.second  is the package pre/post-install scripts
  //!                         execution info
  const
    list< pair< pkgname_t, pkgRunScriptsState_t > >& installFailed()
    const;

  //! \brief   Get the list of packages that were removing failed
  //!
  //! \return  the list of pairs, where
  //!          - pair.first   is the package name
  //!          - pair.second  is the package pre/post-remove scripts
  //!                         execution info
  const
    list< pair< pkgname_t, pkgRunScriptsState_t > >& removeFailed()
    const;

  //! \brief   Get PKGMK_PACKAGE_DIR value from pkgmk(8)
  //!
  //! \return  the PKGMK_PACKAGE_DIR value read from pkgmk(8)
  static string getPkgmkPackageDir();

  //! \brief   Get PKGMK_COMPRESSION_MODE value from pkgmk(8)
  //!
  //! \return  the PKGMK_COMPRESSION_MODE value read from pkgmk(8),
  //!          or "gz" if pkgmk(8) has no such setting.
  static string getPkgmkCompressionMode();

  //! \brief   Get the string describing the transaction error code
  //!
  //! \note    If the argument is passed and it's not equal to
  //!          0 (SUCCESS), then describes this argument, otherwise,
  //!          get the last transaction result stored in class member
  //!          \a m_transactionResult and describes it.
  //!
  //! \param   result  optional transaction \a Result_t value
  //!
  //! \return  a string that describes the transaction error code, end
  //!          empty string if there is no error
  const string strerror( const Result_t& result=SUCCESS ) const;

private:
  //! Repository to look for packages
  const Repository* m_repo;

  //! Package database of installed packages
  PkgDB* m_pkgDB;

  //! Argument parser
  const ArgParser* m_parser;

  //! Configuration file parser
  const Config* m_config;

  //! Package locker
  Locker* m_locker;

  //! Dependency resolver
  DepResolver m_resolver;

  //! Type of executed transaction
  Transaction_t m_transactionType;

  //! Result of executed transaction
  Result_t m_transactionResult;

  //! Log file descriptor
  int m_logfd;

  //! Boolean used to implement lazy initialization
  bool m_depCalced;

  //! Packages to be installed
  list< pair< pkgname_t, const Package* > > m_packages;

  //! Packages that should be installed to meet the requirements
  //! for the packages to be installed, includes the packages
  //! to be installed.
  list< pkgname_t > m_depNameList;

  //! Vector of dependencies that will help to fill in the correct
  //! order \a m_depNameList
  vector< pkgname_t > m_depList;

  //! Packages that were installed in this transaction
  list< pair< pkgname_t, pkgRunScriptsState_t > > m_installedPackages;

  //! Packages that were removed in this transaction
  list< pair< pkgname_t, pkgRunScriptsState_t > > m_removedPackages;

  //! Packages that were requested to be installed
  //! but are already installed
  list< pkgname_t > m_alreadyInstalledPackages;

  //! Packages that were ignored in this transaction,
  //! or selected to be ignored by the user through command-line
  list< pkgname_t > m_ignoredPackages;

  //! Packages that cannot be installed because they could not be found
  //! in the packages sources tree
  list< pair< pkgname_t, pkgname_t > > m_missingPackages;

  //! Packages whose source(s) could not be downloaded
  list< pkgname_t > m_downloadFailedPackages;

  //! Packages that were building failed
  list< pkgname_t > m_buildFailedPackages;

  //! Packages that were installation/update failed
  list< pair< pkgname_t, pkgRunScriptsState_t > >
    m_installFailedPackages;

  //! Packages that were removing failed
  list< pair< pkgname_t, pkgRunScriptsState_t > >
    m_removeFailedPackages;

  //! \brief   Get the log file path from path pattern defined in the
  //!          configuration file
  //!
  //! \param   pkg  the package
  //!
  //! \return  the file path to the \a pkg package log
  //!
  //! \see     Config::logFilePattern()
  inline string logPathFromPattern( const Package* pkg );

  //! \brief   Create the necessary directories for the log file
  //!          placement
  //!
  //! \param   logFilePath  the log file path
  //!
  //! \return  \a true if success, \a false otherwise
  bool logDirCreate( const string& logFilePath );

  //! \brief   Create the log file
  //!
  //! \param   logFilePath  the log file path
  //!
  //! \return  \a true if success, \a false otherwise
  //!
  //! \note    If \a logFilePath already exists then returns \a false
  //!          and set errno to EEXIST, otherwise returns \a true.
  //!          The `--force' command-line option tells to create
  //!          the log file, even if it already exists.
  bool logFileCreate( const string& logFilePath );

  //! \brief   Build the package (pkgmk wrapper)
  //!
  //! \param   pkg    the package to build
  //! \param   logfd  the file descriptor for logging a build process
  //!
  //! \return  \a Result_t value indicating the package building result
  Result_t pkgmk( const Package* pkg ) const;

  //! \brief   Install the package (pkgadd wrapper)
  //!
  //! \param   pkg     the package to install
  //! \param   logfd   the file descriptor for logging an installation
  //!                  process
  //!
  //! \return  \a Result_t  value indicating the package installing
  //!                       result
  Result_t pkgadd( const Package* pkg ) const;

  //! \brief   Calculate dependencies for this transaction
  //!
  //! \return  \a true on success, \a false otherwise
  bool calculateDependencies();

  //! \brief   Recursive method to calculate dependencies
  //!
  //! \param   pkg      the package for which we want to calculate
  //!                   dependencies
  //! \param   depends  index if the package \a pkg depends on,
  //!                   -1 for none
  void checkDependecies( const Package* pkg, ssize_t depends=-1 );
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
