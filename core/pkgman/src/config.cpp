//! \file       config.cpp
//! \brief      Configuration Parser Implementation

#include <filesystem>
#include <fstream>
#include <iostream>

#include "argparser.h"
#include "config.h"
#include "helpers.h"

namespace fs = std::filesystem;

using namespace std;
using namespace StringHelper;

Config::Config( const string& configFile, const ArgParser* parser ):
  m_configFile( configFile ),
  m_parser( parser ),
  m_rootList(),
  m_logFilePattern( "" ),
  m_writeLog( false ),
  m_logMode( OVERWRITE_MODE ),
  m_removeLogOnSuccess( false ),
  m_readmeMode( VERBOSE_README ),
  m_preferHigher( false ),
  m_useRegex( false ),
  m_runScripts( false ),
  m_runscriptCommand( "sh" ),
  m_makeCommand( _PATH_PKGMK_BIN ),
  m_addCommand( _PATH_PKGADD_BIN ),
  m_removeCommand( _PATH_PKGRM_BIN )
{
}

bool Config::parse()
{
  ifstream file( m_configFile );
  if ( ! file.is_open() )
    return false;

  string line;
  while ( getline( file, line ) )
    parseLine( stripWhiteSpace( getValueBefore( line, '#' ) ) );

  file.close();

  //! \note Command line argument \a writeLog overrides config
  if ( m_parser->writeLog() )
    m_writeLog = true;

  return true;
}

void Config::addConfig( const string&  line,
                        bool           configSet,
                        bool           configPrepend )
{
  if ( configSet && startsWithNoCase( line, "pkgsrcdir" ) )
    m_rootList.clear();

  parseLine( line, configPrepend );
}

bool Config::writeLog() const
{
  return m_writeLog;
}

Config::logMode_t Config::logMode() const
{
  return m_logMode;
}

bool Config::removeLogOnSuccess() const
{
  return m_removeLogOnSuccess;
}

string Config::logFilePattern() const
{
  return m_logFilePattern;
}

const rootList_t& Config::rootList() const
{
  return m_rootList;
}

Config::readmeMode_t Config::readmeMode() const
{
  return m_readmeMode;
}

bool Config::preferHigher() const
{
  return m_preferHigher;
}

bool Config::useRegex() const
{
  return m_useRegex;
}

bool Config::runScripts() const
{
  return m_runScripts;
}

string Config::runscriptCommand() const
{
  return m_runscriptCommand;
}

string Config::makeCommand() const
{
  return m_makeCommand;
}

string Config::addCommand() const
{
  return m_addCommand;
}

string Config::removeCommand() const
{
  return m_removeCommand;
}

void Config::parseLine( const string& line, bool prepend )
{
  if ( line.empty() )
    return;

  string s = line;

  if ( startsWithNoCase( s, "pkgsrcdir" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 9, "" ) );
    string path = stripWhiteSpace( getValueBefore( s, ':' ) );
    string packages = getValue( s, ':' );

    if ( fs::is_directory( fs::path( path ) ) )
    {
      if ( prepend )
        m_rootList.push_front( make_pair( path, packages ) );
      else
        m_rootList.push_back(  make_pair( path, packages ) );
    }
    else
      cerr << "pkgman: [config error]: can't access " << path << endl;
  }

  else if ( startsWithNoCase( s, "writelog" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 8, "" ) );
    if ( s == "enabled" )
    {
      // it's already set to false, so we can just enable it.
      // like this, the command line switch works as well
      m_writeLog = true;
    }
  }

  else if ( startsWithNoCase( s, "logmode" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 7, "" ) );
    if ( s == "overwrite" )
    {
      m_logMode = OVERWRITE_MODE;
    }
    else if ( s == "append" )
    {
      m_logMode = APPEND_MODE;
    }
  }

  else if ( startsWithNoCase( s, "logfile" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 7, "" ) );
    m_logFilePattern = s;
  }

  else if ( startsWithNoCase( s, "rmlog_on_success" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 16, "" ) );
    if ( s == "yes" )
      m_removeLogOnSuccess = true;
  }

  else if ( startsWithNoCase( s, "readme" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 6, "" ) );
    if ( s == "compact" )
      m_readmeMode = COMPACT_README;
    else if ( s == "disabled" )
      m_readmeMode = WITHOUT_README;
  }

  else if ( startsWithNoCase( s, "runscripts" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 10, "" ) );
    if ( s == "yes" )
      m_runScripts = true;
  }

  else if ( startsWithNoCase( s, "preferhigher" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 12, "" ) );
    if ( s == "yes" )
      m_preferHigher = true;
  }

  else if ( startsWithNoCase( s, "useregex" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 8, "" ) );
    if ( s == "yes" )
      m_useRegex = true;
  }

  else if ( startsWithNoCase( s, "makecommand" ) )
    m_makeCommand = stripWhiteSpace( s.replace( 0, 11, "" ) );

  else if ( startsWithNoCase( s, "addcommand" ) )
    m_addCommand = stripWhiteSpace( s.replace( 0, 10, "" ) );

  else if ( startsWithNoCase( s, "removecommand" ) )
    m_removeCommand = stripWhiteSpace( s.replace( 0, 13, "" ) );

  else if ( startsWithNoCase( s, "runscriptcommand" ) )
    m_runscriptCommand = stripWhiteSpace( s.replace( 0, 16, "" ) );

  else
    cerr << "pkgman: [config error]: unknown key/value: " << s << endl;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
