//! \file       package.cpp
//! \brief      Package and PackageData Implementation

#include <cstdio>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <sys/stat.h>
#include <sys/utsname.h>

#include "package.h"
#include "helpers.h"

namespace fs = std::filesystem;

using namespace std;

using namespace StringHelper;

Package::Package( const string& name, const string& path ):
  m_loaded( false )
{
  m_data = new PackageData( name, path );
}

Package::Package( const string&  name,
                  const string&  path,
                  const string&  version,
                  const string&  release,
                  const string&  description,
                  const string&  dependencies,
                  const string&  url,
                  const string&  packager,
                  const string&  maintainer,
                  const string&  hasReadme,
                  const string&  hasPreInstall,
                  const string&  hasPostInstall,
                  const string&  hasPreRemove,
                  const string&  hasPostRemove ):
  m_loaded( true )
{
  m_data = new PackageData( name,
                            path,
                            version,
                            release,
                            description,
                            dependencies,
                            url,
                            packager,
                            maintainer,
                            hasReadme,
                            hasPreInstall,
                            hasPostInstall,
                            hasPreRemove,
                            hasPostRemove );
}

Package::~Package()
{
  delete m_data;
}

const string& Package::name() const
{
  return m_data->name;
}

const string& Package::path() const
{
  return m_data->path;
}

const string Package::fullpath( const string& file ) const
{
  if ( file.empty() )
    return m_data->path + '/' + m_data->name;

  return m_data->path + '/' + m_data->name + '/' + file;
}

const string& Package::version() const
{
  load();
  return m_data->version;
}

const string& Package::release() const
{
  load();
  return m_data->release;
}

string Package::version_release() const
{
  load();
  return m_data->version_release;
}

const string& Package::description() const
{
  load();
  return m_data->description;
}

const string& Package::dependencies() const
{
  load();
  return m_data->depends;
}

const string& Package::url() const
{
  load();
  return m_data->url;
}

const string& Package::packager() const
{
  load();
  return m_data->packager;
}

const string& Package::maintainer() const
{
  load();
  return m_data->maintainer;
}

bool Package::hasReadme() const
{
  load();
  return m_data->hasReadme;
}

bool Package::hasPreInstall() const
{
  load();
  return m_data->hasPreInstall;
}

bool Package::hasPostInstall() const
{
  load();
  return m_data->hasPostInstall;
}

bool Package::hasPreRemove() const
{
  load();
  return m_data->hasPreRemove;
}

bool Package::hasPostRemove() const
{
  load();
  return m_data->hasPostRemove;
}

void Package::setDependencies( const string& dependencies )
{
  m_data->depends = dependencies;
}

void Package::load() const
{
  if ( m_loaded )
    return;

  m_loaded = true;

  ifstream file( fullpath( "Pkgfile" ) );
  if ( ! file.is_open() )
    return;

  string line;
  while ( getline( file, line ) )
  {
    line = stripWhiteSpace( line );

    if ( line.substr( 0, 8 ) == "version=" )
    {
      time_t timeNow;
      time( &timeNow );

      struct utsname unameBuf;
      if ( uname( &unameBuf ) != 0 )
        unameBuf.release[ 0 ] = '\0';

      m_data->version = getValueBefore( getValue( line, '=' ), '#' );
      m_data->version = stripWhiteSpace( m_data->version );

      expandShellCommands( m_data->version, timeNow, unameBuf );
    }
    else if ( line.substr( 0, 8 ) == "release=" )
    {
      m_data->release = getValueBefore( getValue( line, '=' ), '#' );
      m_data->release = stripWhiteSpace( m_data->release );
    }
    else if ( line[ 0 ] == '#' )
    {
      while ( line.size() && (   line[ 0 ] == '#'
                              || line[ 0 ] == ' '
                              || line[ 0 ] == '\t' ) )
      {
        line = line.substr( 1 );
      }

      if ( line.find( ':' ) != string::npos )
      {
        string value = stripWhiteSpace( getValue( line, ':' ) );

        if (      startsWithNoCase( line, "desc" ) )
          m_data->description = value;

        else if ( startsWithNoCase( line, "pack" ) )
          m_data->packager    = value;

        else if ( startsWithNoCase( line, "maint" ) )
          m_data->maintainer  = value;

        else if ( startsWithNoCase( line, "url" ) )
          m_data->url         = value;

        else if ( startsWithNoCase( line, "dep" ) )
        {
          replaceAll( value, " ",  "," );
          replaceAll( value, ",,", "," );

          m_data->depends = value;
        }
      }
    }
  }
  file.close();

  m_data->generateVersionReleaseString();

  if ( fs::exists( fullpath( "README" ) ) )
    m_data->hasReadme = true;

  if ( fs::exists( fullpath( "pre-install" ) ) )
    m_data->hasPreInstall = true;

  if ( fs::exists( fullpath( "post-install" ) ) )
    m_data->hasPostInstall = true;

  if ( fs::exists( fullpath( "pre-remove" ) ) )
    m_data->hasPreRemove = true;

  if ( fs::exists( fullpath( "post-remove" ) ) )
    m_data->hasPostRemove = true;
}

void Package::expandShellCommands( string&               input,
                                   const time_t&         timeNow,
                                   const struct utsname  unameBuf )
{
  // TODO: consider dropping either of the tagsets,
  //       depending on feedback

  static const int TAG_COUNT = 2;
  string startTag[TAG_COUNT] = { "`", "$(" };
  string endTag[TAG_COUNT] = { "`", ")" };

  for ( int i = 0; i < TAG_COUNT; ++i )
  {
    string::size_type pos = 0, dpos = 0;
    while ( ( pos = input.find( startTag[ i ], pos ) )
        != string::npos )
    {
      if ( unameBuf.release )
        input = replaceAll( input,
                            startTag[ i ] + "uname -r" + endTag[ i ],
                            unameBuf.release );

      dpos = input.find( startTag[ i ] + "date" );
      if ( dpos != string::npos )
      {
        // NOTE: currently only works for one date pattern
        string::size_type startpos, endpos;

        endpos = input.find( endTag[ i ], dpos + 1 );
        startpos = input.find( '+', dpos + 1 );

        string format =
          input.substr( startpos + 1, endpos - startpos - 1 );

        // support date '+...' and date "+..."
        size_t len = format.length();
        if ( format[ len - 1 ] == '\'' || format[ len - 1 ] == '"' )
            format = format.substr( 0, len - 1 );

        char timeBuf[ 32 ];
        strftime( timeBuf, 32, format.c_str(), localtime( &timeNow ) );

        input = input.substr( 0, dpos )
              + timeBuf
              + input.substr( endpos + 1 );
      }

      ++pos;
    }
  }
}


PackageData::PackageData( const string&  name_,
                          const string&  path_,
                          const string&  version_,
                          const string&  release_,
                          const string&  description_,
                          const string&  dependencies_,
                          const string&  url_,
                          const string&  packager_,
                          const string&  maintainer_,
                          const string&  hasReadme_,
                          const string&  hasPreInstall_,
                          const string&  hasPostInstall_,
                          const string&  hasPreRemove_,
                          const string&  hasPostRemove_ ):
  name( name_ ),
  path( path_ ),
  version( version_ ),
  release( release_ ),
  description( description_ ),
  depends( dependencies_ ),
  url( url_ ),
  packager( packager_ ),
  maintainer( maintainer_ )
{
  generateVersionReleaseString();

  hasReadme       = ( stripWhiteSpace( hasReadme_      ) == "yes" );
  hasPreInstall   = ( stripWhiteSpace( hasPreInstall_  ) == "yes" );
  hasPostInstall  = ( stripWhiteSpace( hasPostInstall_ ) == "yes" );
  hasPreRemove    = ( stripWhiteSpace( hasPreRemove_   ) == "yes" );
  hasPostRemove   = ( stripWhiteSpace( hasPostRemove_  ) == "yes" );
}

void PackageData::generateVersionReleaseString()
{
  version_release = version + "-" + release;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
