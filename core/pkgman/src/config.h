//! \file      config.h
//! \brief     Configuration Parser Definition

#pragma once

#include <list>
#include <string>

#include "pathnames.h"
#include "repository.h"

using namespace std;

class ArgParser;

//! \class   Config
//! \brief   Configuration Parser
//!
//! This is configuration file parser for pkgman
class Config
{
public:
  //! \brief   Construct a Config object
  //!
  //! \param   configFile  path to the configuration file
  //! \param   parser      the argument parser
  //!
  //! \note    the \--log command-line option overrides config
  Config( const string& configFile, const ArgParser* parser );

  //! \brief Parse the configuration file
  //! \return \a true on success, \a false otherwise
  bool parse();

  //! \brief   Add configuration setting
  //!
  //! This is a helper function to modify pkgman's configuration from the
  //! command-line arguments, such as:
  //!   - \--config-set="..."
  //!   - \--config-append="..."
  //!   - \--config-prepend="..."
  //!
  //! \param   line           the string that passed to the command-line
  //!                         argument \--config-xxx="..."
  //! \param   configSet      \a true if the command-line argument was
  //!                         \--config-set="line"
  //! \param   configPrepend  \a true if the command-line argument was
  //!                         \--config-prepend="line"
  void addConfig( const string& line,
                  bool          configSet,
                  bool          configPrepend );

  //! \brief   Whether pkgman should write to a log file
  //!
  //! \return  \a true if so, \a false otherwise
  bool writeLog() const;


  //! Log modes
  enum logMode_t
  {
    APPEND_MODE,      //!<  append the transaction log to log file
    OVERWRITE_MODE,   //!<  overwrite the log file
  };

  //! \brief   Which Log mode should pkgman to use
  //!
  //! \return  \a logMode_t mode
  logMode_t logMode() const;

  //! \brief   Whether pkgman should remove the log file on success build
  //!
  //! \return  \a true if so, \a false otherwise
  bool removeLogOnSuccess() const;

  //! \brief   A path pattern to the log file
  //!
  //! \return  the string that contains a path pattern to the log file
  string logFilePattern() const;

  //! \brief   A path and packages
  //!
  //! \return  the list of pair with path and packages specified by
  //!          "pkgsrcdir"
  const list< pair< string, string > >& rootList() const;

  //! README modes
  enum readmeMode_t
  {
    VERBOSE_README,   //!<  Separate information about README files
    COMPACT_README,   //!<  Integrate in result output
    WITHOUT_README,   //!<  No information about README files
  };

  //! \brief   Which README mode should pkgman to use
  //!
  //! \return  \a readmeMode_t mode
  readmeMode_t readmeMode() const;

  //! \brief   Whether pkgman should parse version strings and prefer the
  //!          higher one, even if the one found in the packages sources
  //!          tree is lower
  //!
  //! \return  \a true if so, \a false otherwise
  bool preferHigher() const;

  //! \brief   Whether pkgman should interpret \b list, \b printf,
  //!          \b search
  //!          arguments as regular expressions
  //!
  //! \return  \a true if so, \a false otherwise
  bool useRegex() const;

  //! \brief   Whether pkgman should run pre/post- scripts
  //!
  //! The following scripts assumed:
  //!   - pre-install
  //!   - post-install
  //!   - pre-remove
  //!   - post-remove
  //!
  //! \return  \a true if should run, \a false otherwise
  bool runScripts() const;

  //! \brief   A path to the scripts interpreter
  //!
  //! \return  the command to run the pre/post- scripts
  string runscriptCommand() const;

  //! \brief   A path to the package builder script
  //!
  //! \return  the command to build the package
  string makeCommand() const;

  //! \brief   A path to the package installer script
  //!
  //! \return  the command to install the package
  string addCommand() const;

  //! \brief   A path to the package remover script
  //!
  //! \return  the command to remove the package
  string removeCommand() const;

private:
  //! path to the pkgman's configuration file
  string m_configFile;

  //! argument parser
  const ArgParser* m_parser;

  //! the list of pair with path and packages
  list< pair< string, string > > m_rootList;

  //! path pattern to the log file
  string m_logFilePattern;

  //! whether write to a log file the build process
  bool m_writeLog;

  //! Log mode
  logMode_t m_logMode;

  //! whether remove the log file after success build
  bool m_removeLogOnSuccess;

  //! README mode
  readmeMode_t m_readmeMode;

  //! whether parse version strings and prefer the higher one even if
  //! the one found in the packages sources tree is lower
  bool m_preferHigher;

  //! whether interpret search strings and wildcard patterns as regex
  bool m_useRegex;

  //! whether run pre/post-install and pre/post-remove scripts
  bool m_runScripts;

  //! the command to run the pre/post-install and pre/post-remove scripts
  string m_runscriptCommand;

  //! the command to build the package
  string m_makeCommand;

  //! the command to install the package
  string m_addCommand;

  //! the command to remove the package
  string m_removeCommand;

  //! \brief   Parse the config file line
  //!
  //! Parse a line and set the configuration data accordingly;
  //! if \a prepend is set, prepend the data for configuration options
  //! which are lists.
  //!
  //! \param   line     the string to be parsed
  //! \param   prepend  whether prepend the data
  void parseLine( const string& line, bool prepend=false );
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
