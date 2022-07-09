//! \file       argparser.cpp
//! \brief      ArgParser class implementation

#include "argparser.h"

using namespace std;

ArgParser::ArgParser( int argc, char** argv ):
  m_isCommandGiven( false ),
  m_commandID( -1 ),
  m_commandName( "" ),
  m_unknownOption( "" ),
  m_Help( false ),
  m_Version( false ),
  m_isForced( false ),
  m_isTest( false ),
  m_isAlternateConfigGiven( false ),
  m_hasFilter( false ),
  m_noStdConfig( false ),
  m_writeLog( false ),
  m_deps( false ),
  m_all( false ),
  m_full( false ),
  m_printPath( false ),
  m_execPreInstall( false ),
  m_execPostInstall( false ),
  m_execPreRemove( false ),
  m_execPostRemove( false ),
  m_useRegex( false ),
  m_recursive( false ),
  m_printTree( false ),
  m_group( false ),
  m_depSort( false ),
  m_alternateConfigFile( "" ),
  m_pkgmkArgs( "" ),
  m_pkgaddArgs( "" ),
  m_pkgrmArgs( "" ),
  m_sortArgs( "" ),
  m_filter( "" ),
  m_root( "" ),
  m_ignore( "" ),
  m_argc( argc ),
  m_argv( argv ),
  m_verbose( 0 )
{
}

bool ArgParser::parse()
{
  if ( m_argc < 2 )
    return false;

  string commands[] = {
    // Informational
    "dumpconfig", "list", "list-dup", "list-nodependents", "list-orphans",
    "list-locked", "printf", "info", "readme", "path", "isinst", "current",
    // Differences / Check for updates
    "diff",
    // Dependencies
    "mdep", "dep", "rdep",
    // Searching
    "search", "dsearch", "fsearch",
    // Install / Update / Remove
    "install", "update", "remove",
    // System update
    "sysup", "lock", "unlock",
    // File operations
    "ls", "cat", "edit",
  };

  constexpr size_t commands_size = std::end(commands) - std::begin(commands);

  // parse options
  for ( int i = 1; i < m_argc; ++i )
  {
    if ( m_argv[ i ][ 0 ] == '-' )
    {
      string arg = m_argv[ i ];

      if ( arg == "--no-std-config" )
        m_noStdConfig = true;

      else if ( arg.substr( 0, 9 ) == "--config=" )
      {
        m_alternateConfigFile = arg.substr( 9 );
        m_isAlternateConfigGiven = true;
      }

      else if ( arg.substr( 0, 16 ) == "--config-append=" )
        m_configData.push_back( make_pair( m_argv[ i ] + 16, CONFIG_APPEND  ) );

      else if ( arg.substr( 0, 17 ) == "--config-prepend=" )
        m_configData.push_back( make_pair( m_argv[ i ] + 17, CONFIG_PREPEND ) );

      else if ( arg.substr( 0, 13 ) == "--config-set=" )
        m_configData.push_back( make_pair( m_argv[ i ] + 13, CONFIG_SET     ) );

      else if ( arg.substr( 0, 7 ) == "--root=" )
        m_root = arg.substr( 7 );

      else if ( arg == "-v" )
        m_verbose += 1;

      else if ( arg == "-vv" )
        m_verbose += 2;

      else if ( arg == "-h" || arg == "--help" )
        m_Help = true;

      else if ( arg == "-V" || arg == "--version" )
        m_Version = true;

      else if ( arg == "--force" )
        m_isForced = true;

      else if ( arg == "--test" )
        m_isTest = true;

      else if ( arg == "--deps" )
        m_deps = true;

      else if ( arg == "--all" )
        m_all = true;

      else if ( arg == "--full" )
        m_full = true;

      else if ( arg == "--path" )
        m_printPath = true;

      else if ( arg == "--log" )
        m_writeLog = true;

      else if ( arg == "--pre-install" )
        m_execPreInstall = true;

      else if ( arg == "--post-install" )
        m_execPostInstall = true;

      else if ( arg == "--install-scripts" )
        m_execPreInstall = m_execPostInstall = true;

      else if ( arg == "--pre-remove" )
        m_execPreRemove = true;

      else if ( arg == "--post-remove" )
        m_execPostRemove = true;

      else if ( arg == "--remove-scripts" )
        m_execPreRemove = m_execPostRemove = true;

      else if ( arg == "--no-std-config" )
        m_noStdConfig = true;

      else if ( arg == "--regex" )
        m_useRegex = true;

      else if ( arg == "--recursive" )
        m_recursive = true;

      else if ( arg == "--tree" )
        m_printTree = true;

      else if ( arg == "--group" )
        m_group = true;

      else if ( arg == "--depsort" )
        m_depSort = true;

      // pkgadd arguments aliases
      else if ( arg == "-f" )
        m_pkgaddArgs += " -f";

      else if ( arg == "-fi" )
        m_pkgaddArgs += " -f";

      // pkgmk arguments aliases
      else if ( arg == "-d" )
        m_pkgmkArgs += " -d";

      else if ( arg == "-do" )
        m_pkgmkArgs += " -do";

      else if ( arg == "-fr" )
        m_pkgmkArgs += " -f";

      else if ( arg == "-if" )
        m_pkgmkArgs += " -if";

      else if ( arg == "-uf" )
        m_pkgmkArgs += " -uf";

      else if ( arg == "-im" )
        m_pkgmkArgs += " -im";

      else if ( arg == "-um" )
        m_pkgmkArgs += " -um";

      else if ( arg == "-is" )
        m_pkgmkArgs += " -is";

      else if ( arg == "-us" )
        m_pkgmkArgs += " -us";

      else if ( arg == "-kw" )
        m_pkgmkArgs += " -kw";

      else if ( arg == "-ns" )
        m_pkgmkArgs += " -ns";

      // substrings
      else if ( arg.substr( 0, 8 ) == "--margs=" )
        m_pkgmkArgs += " " + arg.substr( 8 );

      else if ( arg.substr( 0, 8 ) == "--aargs=" )
        m_pkgaddArgs += " " + arg.substr( 8 );

      else if ( arg.substr( 0, 8 ) == "--rargs=" )
        m_pkgrmArgs = arg.substr( 8 );

      else if ( arg.substr( 0, 7 ) == "--sort=" )
        m_sortArgs = arg.substr( 7 );

      else if ( arg.substr( 0, 9 ) == "--filter=" )
      {
        m_filter = arg.substr( 9 );
        m_hasFilter = true;
      }

      else if ( arg.substr( 0, 9 ) == "--ignore=" )
        m_ignore = arg.substr( 9 );

      else
      {
        m_unknownOption = arg;
        return false;
      }
    }
    else
    {
      if ( ! m_isCommandGiven )
      {
        string arg = m_argv[i];
        m_commandName = arg;

        for ( size_t j = 0; j < commands_size; j++ )
        {
          if ( arg == commands[j] )
          {
            m_isCommandGiven = true;
            m_commandID = j;
            break;
          }
        }

        // fist argument must be command
        if ( ! m_isCommandGiven )
          return false;
      }
      else
        m_cmdArgs.push_back( m_argv[i] );
    }
  } // for

  return m_isCommandGiven;
}

bool ArgParser::isCommandGiven() const
{
  return m_isCommandGiven;
}

size_t ArgParser::commandID() const
{
  return m_commandID;
}

bool ArgParser::isHelp() const
{
  return m_Help;
}

bool ArgParser::isVersion() const
{
  return m_Version;
}

bool ArgParser::isForced() const
{
  return m_isForced;
}

bool ArgParser::isTest() const
{
  return m_isTest;
}

bool ArgParser::isAlternateConfigGiven() const
{
  return m_isAlternateConfigGiven;
}

bool ArgParser::writeLog() const
{
  return m_writeLog;
}

bool ArgParser::hasFilter() const
{
  return m_hasFilter;
}

bool ArgParser::noStdConfig() const
{
  return m_noStdConfig;
}

bool ArgParser::deps() const
{
  return m_deps;
}

bool ArgParser::all() const
{
  return m_all;
}

bool ArgParser::full() const
{
  return m_full;
}

bool ArgParser::printPath() const
{
  return m_printPath;
}

bool ArgParser::execPreInstall() const
{
  return m_execPreInstall;
}

bool ArgParser::execPostInstall() const
{
  return m_execPostInstall;
}

bool ArgParser::execPreRemove() const
{
  return m_execPreRemove;
}

bool ArgParser::execPostRemove() const
{
  return m_execPostRemove;
}

bool ArgParser::useRegex() const
{
  return m_useRegex;
}

bool ArgParser::recursive() const
{
  return m_recursive;
}

bool ArgParser::printTree() const
{
  return m_printTree;
}

bool ArgParser::group() const
{
  return m_group;
}

bool ArgParser::depSort() const
{
  return m_depSort;
}

const string& ArgParser::alternateConfigFile() const
{
  return m_alternateConfigFile;
}

const string& ArgParser::pkgmkArgs() const
{
  return m_pkgmkArgs;
}

const string& ArgParser::pkgaddArgs() const
{
  return m_pkgaddArgs;
}

const string& ArgParser::pkgrmArgs() const
{
  return m_pkgrmArgs;
}

const string& ArgParser::sortArgs() const
{
  return m_sortArgs;
}

const string& ArgParser::filter() const
{
  return m_filter;
}

const string& ArgParser::root() const
{
  return m_root;
}

const string& ArgParser::ignore() const
{
  return m_ignore;
}

const string& ArgParser::commandName() const
{
  return m_commandName;
}

const string& ArgParser::unknownOption() const
{
  return m_unknownOption;
}

const list< char* >& ArgParser::cmdArgs() const
{
  return m_cmdArgs;
}

int ArgParser::verbose() const
{
  return m_verbose;
}

const
list< pair< char*, ArgParser::configArg_t > > ArgParser::configData()
  const
{
  return m_configData;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
