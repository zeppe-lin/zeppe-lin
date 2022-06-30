//! \file      pkgmksetting.cpp
//! \brief     Get settings from pkgmk configuration

#include <fstream>

#include "argparser.h"
#include "helpers.h"
#include "pkgmksetting.h"

using namespace std;
using namespace StringHelper;

PkgmkSetting::PkgmkSetting( const ArgParser* parser ) :
  m_parser( parser )
{
}

const string PkgmkSetting::get( const string& setting )
{
  using namespace StringHelper;

  size_t npos;
  string configFile;

  // if pkgman was called with "--margs='-cf ...' ", that means
  // new pkgmk configuration file declared.
  if ( ( npos = m_parser->pkgmkArgs().find("-cf ") ) != string::npos )
    configFile =
      getValueBefore( m_parser->pkgmkArgs().substr( npos + 4 ), ' ' );
  else
    configFile = _PATH_CONF;

  string value = getSettingFromFile( setting, _PATH_PKGMK_CONF );
  if ( value.size() )
    return value;

  // makeCommand can be overridden by pkgman.conf, so rely on
  // the default pkgmk location.
  return getSettingFromFile( setting, _PATH_PKGMK_BIN );
}

string PkgmkSetting::getSettingFromFile( const string& setting,
                                         const string& filename )
{
  using namespace StringHelper;
  ifstream file( filename );
  if ( ! file.is_open() )
    return "";

  string candidate;
  string line;
  while ( getline( file, line ) )
  {
    line = stripWhiteSpace( line );
    if ( startsWith( line, setting + "=" ) )
      candidate = line;
  }
  file.close();

  if ( candidate.empty() )
    return "";

  string cmd = "eval " + candidate + " && echo $" + setting;

  FILE* p = popen( cmd.c_str(), "r" );
  if ( ! p )
    return "";

  // XXX fix the limit of 256 chars?
  char outline[ 256 ];
  fgets( outline, 256, p );
  fclose( p );

  return stripWhiteSpace( outline );
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
