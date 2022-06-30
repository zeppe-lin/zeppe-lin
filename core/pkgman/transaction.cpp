//! \file       transaction.cpp
//! \brief      Transaction class implementation

#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "argparser.h"
#include "config.h"
#include "helpers.h"
#include "pkgdb.h"
#include "pkgmksetting.h"
#include "process.h"
#include "transaction.h"

namespace fs = std::filesystem;

using namespace std;
using namespace ListHelper;
using namespace StringHelper;

const Transaction::Transaction_t& Transaction::type() const
{
  return m_transactionType;
}

const Transaction::Result_t& Transaction::result() const
{
  return m_transactionResult;
}

const Transaction::Result_t&
  Transaction::install( Transaction::Transaction_t transactionType )
{
  m_transactionType = transactionType;

  if ( m_packages.empty() )
    return m_transactionResult = NO_PACKAGE_GIVEN;

  // Get ignored packages for this transaction which are indicated by
  // the user at the command-line
  list< string > ignoredPackages;
  split( m_parser->ignore(), ',', ignoredPackages );

  for ( const auto& [ name, pkg ]: m_packages )
  {
    // skip if ignored
    if ( contains( ignoredPackages, name ) )
    {
      m_ignoredPackages.push_back( name );
      continue;
    }

    // skip or return an error (depends on `--group' command-line
    // argument) if trying to install/update the package that is
    // missing in the packages sources tree
    if ( ! pkg )
    {
      m_missingPackages.push_back( make_pair( name, "" ) );

      if ( m_parser->group() )
        return m_transactionResult = PACKAGE_NOT_FOUND;

      continue;
    }

    // skip if trying to install already installed package
    if ( transactionType == INSTALL && m_pkgDB->isInstalled( name ) )
    {
      m_alreadyInstalledPackages.push_back( name );
      continue;
    }

    const auto& logFilePath = logPathFromPattern( pkg );

    // Create a transaction log file if logging is enabled,
    // and skip otherwise.
    // Also, skip this if `--test' command-line option is set.
    if ( ! m_parser->isTest() && m_config->writeLog() )
    {
      // critical errors, return if occurs
      if ( logFilePath.empty() )
        return m_transactionResult = NO_LOG_FILE;

      if ( ! logDirCreate( logFilePath ) )
        return m_transactionResult = LOG_DIR_FAILURE;

      if ( ! logFileCreate( logFilePath ) )
      {
        // If the log file can't be created, but the reason _is not_
        // that it already exists -- this is a critical error.
        if ( errno != EEXIST )
          return m_transactionResult = LOG_FILE_FAILURE;
        else
          cerr << "pkgman: [warning]: log file already exists" << endl;

        // The log file already exists, skip building this package.
        // It's not a critical error: maybe another `pkgman' instance
        // uses this log file or the previous transaction process was
        // interrupted.
        m_ignoredPackages.push_back( name );

        // But if user want to interrupt the whole process in this case...
        if ( m_parser->group() )
          return m_transactionResult = LOG_FILE_EEXIST;

        // warn the user about skipping...
        cout << "pkgman: " << name << ": "
          << strerror( LOG_FILE_EEXIST ) << endl
          << "pkgman: " << name << ": skip\n";

        continue;
      }
    }

    bool isDownloadOnly = contains( m_parser->pkgmkArgs(), "-do" );

    pkgRunScriptsState_t scriptsInfo;
    Result_t pkgmkResult, pkgaddResult;

    // build the package unless `--test' command-line option is set
    pkgmkResult = m_parser->isTest() ? SUCCESS : pkgmk( pkg );

    if ( ! isDownloadOnly && pkgmkResult == SUCCESS )
    {
      // `pre-install` script execution
      if ( ( m_parser->execPreInstall() || m_config->runScripts() )
          && pkg->hasPreInstall() )
      {
        Process pre(
            m_config->runscriptCommand(),
            pkg->fullpath( "pre-install" ),
            m_logfd,                 /* the log file descriptor */
            m_parser->verbose() > 1  /* log to stdout if true   */ );

        // execute `pre-install' script unless `--test' command-line
        // option is set
        scriptsInfo.pre =
          m_parser->isTest() ? SUCCESS : pre.executeShell();
      }

      // install the package
      pkgaddResult = m_parser->isTest() ? SUCCESS : pkgadd( pkg );
      if ( pkgaddResult == SUCCESS )
      {
        // `post-install' script execution
        if ( ( m_parser->execPostInstall() || m_config->runScripts() )
            && pkg->hasPostInstall() )
        {
          Process post( m_config->runscriptCommand(),
                        pkg->fullpath( "post-install" ),
                        m_logfd,
                        m_parser->verbose() > 1 );
          scriptsInfo.post =
            m_parser->isTest() ? SUCCESS : post.executeShell();
        }

        m_installedPackages.push_back(
            make_pair( name, scriptsInfo ) );
      }
      else
        m_installFailedPackages.push_back(
            make_pair( name, scriptsInfo ) );
    }
    else if ( pkgmkResult == PKGMK_E_DOWNLOAD )
      m_downloadFailedPackages.push_back( name );
    else if ( ! isDownloadOnly )
      m_buildFailedPackages.push_back( name );

    if ( ! m_parser->isTest() && m_config->writeLog() )
    {
      // Remove the log file if `pkgman' is configured to remove a log
      // after successful operation.
      if (     m_config->removeLogOnSuccess()
          &&   pkgmkResult  == SUCCESS
          && ( pkgaddResult == SUCCESS || isDownloadOnly ) )
        unlinkat( m_logfd, logFilePath.c_str(), 0 );
      else
        close( m_logfd );
    }

    if ( pkgmkResult != SUCCESS )
    {
      if ( m_parser->group() )
        return m_transactionResult = pkgmkResult;

      cout << "pkgman: makecommand: " << name << ": "
           << strerror( pkgmkResult ) << endl;
    }

    if ( ! isDownloadOnly && pkgaddResult != SUCCESS )
    {
      if ( m_parser->group() )
        return m_transactionResult = pkgaddResult;

      cout << "pkgman: addcommand: " << name << ": "
           << strerror( pkgaddResult ) << endl;
    }
  }
  return m_transactionResult = SUCCESS;
}

const Transaction::Result_t&
Transaction::remove()
{
  m_transactionType = REMOVE;

  if ( m_packages.empty() )
    return m_transactionResult = NO_PACKAGE_GIVEN;

  const string& pkgrm = m_config->removeCommand();
  string pkgrmArgs;

  if ( m_parser->root().size() )
    pkgrmArgs = "-r " + m_parser->root() + " " + m_parser->pkgrmArgs();
  else if ( contains( pkgrmArgs , "-r" ) )
    pkgrmArgs = m_parser->pkgrmArgs();

  list< string > ignoredPackages;
  split( m_parser->ignore(), ',', ignoredPackages );

  for ( const auto& [ name, pkg ]: m_packages )
  {
    // skip if ignored
    if ( contains( ignoredPackages, name ) )
    {
      m_ignoredPackages.push_back( name );
      continue;
    }

    // also, skip if the package isn't installed or missing
    // in the packages sources tree
    if ( ! m_pkgDB->isInstalled( name ) || ! pkg )
    {
      m_missingPackages.push_back( make_pair( name, "" ) );

      if ( m_parser->group() )
        return m_transactionResult = PACKAGE_NOT_FOUND;

      continue;
    }

    pkgRunScriptsState_t scriptsInfo;

    // `pre-remove' script execution
    Process pre( m_config->runscriptCommand(),
                 pkg->fullpath( "pre-remove" ),
                 -1 /* no log file descriptor */,
                 m_parser->verbose() > 1 /* log to stdout if true */ );

    if ( ( m_parser->execPreRemove() || m_config->runScripts() )
        && pkg->hasPreRemove() )
      scriptsInfo.pre =
        m_parser->isTest() ? SUCCESS : pre.executeShell();

    // removing the package
    Process pkgrmProc( pkgrm,
                       pkgrmArgs + " " + name,
                       -1,
                       m_parser->verbose() > 1 );

    const auto& pkgrmResult =
      m_parser->isTest() ? SUCCESS : pkgrmProc.executeShell();

    // `post-remove' script execution
    if ( ( m_parser->execPostRemove() || m_config->runScripts() )
        && pkg->hasPostRemove() )
    {
      Process post(
          m_config->runscriptCommand(),
          pkg->fullpath( "post-remove" ),
          -1 /* no log file descriptor */,
          m_parser->verbose() > 1 /* log to stdout if true */ );

      scriptsInfo.post =
        m_parser->isTest() ? SUCCESS : post.executeShell();
    }

    if ( m_parser->isTest() || pkgrmResult == SUCCESS )
    {
      m_removedPackages.push_back( make_pair( name, scriptsInfo ) );

      if ( m_locker->isLocked( name ) )
        m_locker->unlock( name );
    }
    else
    {
      m_removeFailedPackages.push_back(
          make_pair( name, scriptsInfo ) );

      if ( m_parser->group() )
        return m_transactionResult = PKGRM_E_GENERAL;
    }
  }

  m_locker->store();

  return m_transactionResult = SUCCESS;
}

const Transaction::Result_t& Transaction::calcDeps()
{
  m_transactionType = DEPCALC;

  if ( m_packages.empty() )
    return m_transactionResult = NO_PACKAGE_GIVEN;

  bool validPackages = false;

  for ( const auto& [ name, pkg ]: m_packages )
  {
    if ( pkg )
      validPackages = true;
    else
    {
      // note: moved here from calculateDependencies
      m_missingPackages.push_back( make_pair( name, "" ) );
    }
  }

  if ( ! validPackages )
    return m_transactionResult = PACKAGE_NOT_FOUND;

  if ( ! calculateDependencies() )
    return m_transactionResult = CYCLIC_DEPEND;

  return m_transactionResult = SUCCESS;
}

const list< pkgname_t >& Transaction::deps() const
{
  return m_depNameList;
}

const list< pair< pkgname_t, Transaction::pkgRunScriptsState_t > >&
Transaction::installed() const
{
  return m_installedPackages;
}

const list< pair< pkgname_t, Transaction::pkgRunScriptsState_t > >&
Transaction::removed() const
{
  return m_removedPackages;
}

const list< pkgname_t >& Transaction::alreadyInstalled() const
{
  return m_alreadyInstalledPackages;
}

const list< pkgname_t >& Transaction::ignored() const
{
  return m_ignoredPackages;
}

const list< pair< pkgname_t, pkgname_t > >& Transaction::missing() const
{
  return m_missingPackages;
}

const list< pkgname_t >& Transaction::downloadFailed() const
{
  return m_downloadFailedPackages;
}

const list< pkgname_t >& Transaction::buildFailed() const
{
  return m_buildFailedPackages;
}

const list< pair< pkgname_t, Transaction::pkgRunScriptsState_t > >&
Transaction::installFailed() const
{
  return m_installFailedPackages;
}

const list< pair< pkgname_t, Transaction::pkgRunScriptsState_t > >&
Transaction::removeFailed() const
{
  return m_removeFailedPackages;
}

const string Transaction::strerror( const Result_t& result ) const
{
  switch ( result ? result : m_transactionResult )
  {
    case SUCCESS:
      break;

    case NO_PACKAGE_GIVEN:
      switch ( m_transactionType )
      {
        case UPDATE:
          return "no package specified for update";
        case INSTALL:
          return "no package specified for install";
        case REMOVE:
          return "no package specified for remove";
        case DEPCALC:
          return "no package specified for dependency calculation";
        default:
          return "none transaction type specified";
      }

    case PACKAGE_NOT_FOUND:
      return m_missingPackages.begin()->first + " package not found";

    case CYCLIC_DEPEND:
      return "cyclic dependencies found";

    case NO_LOG_FILE:
      return "no log file specified, but logging enabled";

    case LOG_DIR_FAILURE:
      return "can't create the log file directory for " +
             m_packages.begin()->first;

    case LOG_FILE_FAILURE:
      return "can't create the log file for " +
             m_packages.begin()->first;

    case LOG_FILE_EEXIST:
      return
">> log file already exists!\n"
">  Maybe another process uses it, or the previous build\n"
">  failed/interrupted. It's safer to skip building.\n"
">  Or... remove the log, or use --force option."
      ;

    case PKGMK_E_GENERAL:
      return "error while executing 'makecommand'";

    case PKGMK_E_PKGFILE:
      return "invalid Pkgfile";

    case PKGMK_E_DIR_PERM:
      return "source/build directory missing "
             "or missing read/write permission";

    case PKGMK_E_DOWNLOAD:
      return "error during download";

    case PKGMK_E_UNPACK:
      return "error during unpacking of source file(s)";

    case PKGMK_E_MD5:
      return "md5sum verification failed";

    case PKGMK_E_FOOTPRINT:
      return "footprint check failure";

    case PKGMK_E_BUILD:
      return "error while running `build()'";

    case PKGMK_E_INSTALL:
      return "error while executing 'addcommand'";

    default:
      return "unknown error";
  }
  return "";
}


///////////////////////////////////////////////////////////////////////
//                          private members                          //
///////////////////////////////////////////////////////////////////////


inline string Transaction::logPathFromPattern( const Package* pkg )
{
  string logfile = m_config->logFilePattern();

  replaceAll( logfile, "%n", pkg->name()    );
  replaceAll( logfile, "%p", pkg->path()    );
  replaceAll( logfile, "%v", pkg->version() );
  replaceAll( logfile, "%r", pkg->release() );

  return logfile;
}

bool Transaction::logDirCreate( const string& logFilePath )
{
  return Repository::createOutputDir(
      fs::path( logFilePath ).parent_path() );
}

bool Transaction::logFileCreate( const string& logFilePath )
{
  if ( m_config->logMode() == Config::OVERWRITE_MODE )
  {
    m_logfd =
      open( logFilePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666 );
  }
  else
  {
    m_logfd =
      open( logFilePath.c_str(), O_WRONLY | O_APPEND, 0666 );
  }
  return m_logfd != -1;
}

Transaction::Result_t Transaction::pkgmk( const Package* pkg ) const
{
  const auto& pkgmk     = m_config->makeCommand();
  const auto& pkgmkArgs =
    m_parser->pkgmkArgs().size() ? m_parser->pkgmkArgs() : "-d";

  // inform the user about what's happening
  string message = "pkgman: preparing ";
  message += pkg->name() + " " + pkg->version_release() + "\n";

  cout << message;
  if ( m_config->writeLog() )
    write( m_logfd, message.c_str(), message.length() );

  if ( chdir( pkg->fullpath().c_str() ) != 0 )
  {
    message = "pkgman: can't chdir into " + pkg->fullpath() + "\n";

    if ( m_logfd )
      write( m_logfd, message.c_str(), message.length() );

    return PKGMK_E_GENERAL;
  }

  Process pkgmkProc( pkgmk, pkgmkArgs, m_logfd,
                     m_parser->verbose() > 1 );

  const auto& result = pkgmkProc.executeShell();

  return result != SUCCESS ? static_cast<Result_t>( result ) : SUCCESS;
}

Transaction::Result_t Transaction::pkgadd( const Package* pkg ) const
{
  PkgmkSetting pkgmkConf( m_parser );
  string pkgadd = m_config->addCommand();
  string pkgaddArgs;

  if ( m_parser->root().size() )
  {
    // custom defined install root directory
    pkgaddArgs += "-r " + m_parser->root() + " ";
  }

  if ( m_parser->pkgaddArgs().size() )
    pkgaddArgs += m_parser->pkgaddArgs() + " ";

  if ( m_transactionType == UPDATE )
    pkgaddArgs += "-u ";

  pkgaddArgs += pkgmkConf.get( "PKGMK_PACKAGE_DIR" )
             +  "/"
             +  pkg->name()
             +  "#"
             +  pkg->version_release()
             +  ".pkg.tar."
             +  pkgmkConf.get( "PKGMK_COMPRESSION_MODE" );

  // inform the user about what's happening
  string message = "pkgman: ";
  {
    string name = pkg->name();
    string from = m_pkgDB->getVersionRelease( pkg->name() );
    string to   = pkg->version_release();

    if ( m_transactionType == UPDATE )
    {
      if ( from == to )
        message += "reinstalling " + name + " " + to;
      else
        message += "updating " + name + " " + from + " -> " + to;
    }
    else
      message += "installing " + name + " " + to;

    message += "\n";
  }
  cout << message;
  if ( m_logfd )
    write( m_logfd, message.c_str(), message.length() );

  Process pkgaddProc( pkgadd, pkgaddArgs, m_logfd,
                      m_parser->verbose() > 1 );

  return pkgaddProc.executeShell() != SUCCESS
         ? PKGMK_E_INSTALL
         : SUCCESS;
}

bool Transaction::calculateDependencies()
{
  if ( m_depCalced )
    return true;

  m_depCalced = true;

  for ( const auto& [ name, pkg ]: m_packages)
  {
    if ( pkg )
      checkDependecies( pkg );
  }

  list< ssize_t > indexList;
  if ( ! m_resolver.resolve( indexList ) )
    return m_depCalced = false;

  for ( const auto& index: indexList )
    m_depNameList.push_back( m_depList[ index ] );

  return true;
}

void Transaction::checkDependecies( const Package*  pkg,
                                    ssize_t         depends )
{
  ssize_t index = -1;
  bool newPackage = true;
  for ( unsigned int i = 0; i < m_depList.size(); ++i )
  {
    if ( m_depList[ i ] == pkg->name() )
    {
      index = i;
      newPackage = false;
      break;
    }
  }

  if ( index == -1 )
  {
    index = m_depList.size();
    m_depList.push_back( pkg->name() );
  }

  if ( depends != -1 )
    m_resolver.addDependency( index, depends );
  else
  {
    // this just adds index to the dependency resolver
    m_resolver.addDependency( index, index );
  }

  if ( ! newPackage || pkg->dependencies().empty() )
    return;

  list< string > deps;
  split( pkg->dependencies(), ',', deps );

  for ( string& dep: deps )
  {
    if ( dep.empty() )
      continue;

    // XXX see depresolver
    auto pos = dep.find_last_of( '/' );
    if ( pos != string::npos && ( pos + 1 ) < dep.length() )
      dep = dep.substr( pos + 1 );

    const auto& pkg_dep = m_repo->getPackage( dep );
    if ( pkg_dep )
      checkDependecies( pkg_dep, index );
    else
      m_missingPackages.push_back( make_pair( dep, pkg->name() ) );
  }
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
