//! \file       repository.cpp
//! \brief      Repository class implementation

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "datafileparser.h"
#include "p_regex.h"
#include "repository.h"
#include "helpers.h"

namespace fs = std::filesystem;

using namespace std;
using namespace StringHelper;
using namespace ListHelper;

Repository::Repository( bool useRegex ):
  m_useRegex( useRegex )
{
}

Repository::~Repository()
{
  for ( const auto& [ name, pkgobj ]: m_packageMap )
    delete pkgobj;
}

const Package* Repository::getPackage( const pkgname_t& name ) const
{
  const auto& pkg = m_packageMap.find( name );
  if ( pkg == m_packageMap.end() )
    return 0;
  return pkg->second; /* Package pointer */
}

const map< pkgname_t, Package* >& Repository::packages() const
{
  return m_packageMap;
}

const
list< pair< Package*, Package* > >& Repository::shadowedPackages()
  const
{
  return m_shadowedPackages;
}

void
Repository::searchMatchingPackages( const string&      pattern,
                                    list< Package* >&  target,
                                    bool               searchDesc )
  const
{
  //! \note  \a searchDesc \a true will read _every_ Pkgfile
  if ( m_useRegex )
  {
    RegEx re( pattern );

    for ( const auto& [ pkgname, pkgobj ]: m_packageMap )
      if ( re.match( pkgname ) )
        target.push_back( pkgobj );
      else if ( searchDesc )
      {
        if ( re.match( pkgobj->description() ) )
          target.push_back( pkgobj );
      }
  }
  else
  {
    for ( const auto& [ pkgname, pkgobj ]: m_packageMap )
      if ( contains( pkgname, pattern ) )
        target.push_back( pkgobj );
      else if ( searchDesc )
      {
        string s = toLowerCase( pkgobj->description() );
        if ( contains( s, pattern ) )
          target.push_back( pkgobj );
      }
  }
}

int Repository::compareShadowPair( pair< Package*, Package* >&  p1,
                                   pair< Package*, Package* >&  p2 )
{
  return p1.second->name() < p2.second->name();
}

void Repository::initFromFS( const rootList_t& rootList,
                             bool         listDuplicate )
{
  map< string, bool > alreadyChecked;

  for ( const auto& [ path, pkgs ]: rootList )
  {
    if ( alreadyChecked[ path ] )
      continue;

    string pkgstr = stripWhiteSpace( pkgs );

    bool filter = false;
    if ( pkgstr.size() )
    {
      filter = true;
      replaceAll( pkgstr, " ",  "," );
      replaceAll( pkgstr, "\t", "," );
      replaceAll( pkgstr, ",,", "," );
    }
    if ( ! filter )
        alreadyChecked[ path ] = true;

    list< string > pkglist;
    split( pkgstr, ',', pkglist );

    // TODO: think about whether it would be faster (more
    // efficient) to put all packages into a map, and the iterate
    // over the list of allowed packages and copy them
    // over. depending in the efficiency of find(), this might be
    // faster
    for ( const auto& file: fs::directory_iterator( path ) )
    {
      // TODO: review this
      fs::path pkgname = file.path().filename();
      fs::path pkgfile = file.path() / "Pkgfile";

      if ( std::error_code ec; ! fs::exists( pkgfile, ec ) )
        continue; // no Pkgfile -> no package source

      if ( filter && ! contains( pkglist, pkgname.string() ) )
        continue; // not found -> ignore this package source

      const auto& pkgobj = new Package( pkgname, path );
      if ( ! pkgobj )
        continue;

      const auto& hidden = m_packageMap.find( pkgname );
      if ( hidden == m_packageMap.end() )
        m_packageMap[ pkgname ] = pkgobj; // no such package found, add
      else if ( listDuplicate )
        m_shadowedPackages.push_back(
            make_pair( pkgobj, hidden->second ) );
      else
        delete pkgobj;
    }
  }

  m_shadowedPackages.sort( compareShadowPair );
}

bool Repository::createOutputDir( const string& path )
{
  list< string > dirs;
  split( path, '/', dirs, 1 );

  fs::path tmpPath;
  for ( const string& dir: dirs )
  {
    tmpPath /= dir;
    if ( ! fs::exists( tmpPath ) )
    {
      if ( std::error_code ec; ! fs::create_directory( tmpPath, ec ) )
        return false;
    }
  }
  return true;
}

void Repository::getMatchingPackages( const string&      pattern,
                                      list< Package* >&  target ) const
{
  if ( m_useRegex )
  {
    RegEx re( pattern );

    for ( const auto& [ pkgname, pkgobj ]: m_packageMap )
      if ( re.match( pkgname ) )
        target.push_back( pkgobj );
  }
  else
  {
    for ( const auto& [ pkgname, pkgobj ]: m_packageMap )
    {
      // jw: I assume  fnmatch will be quite fast for "match all" (*),
      // so I didn't add a boolean to check for this explicitely
      if ( fnmatch( pattern.c_str(), pkgname.c_str(), 0 ) == 0 )
        target.push_back( pkgobj );
    }
  }
}

void Repository::addDependencies( map< pkgname_t, pkgname_t >& deps )
{
  for ( const auto& [ first, second ]: deps )
  {
    const auto& it = m_packageMap.find( first );
    if ( it == m_packageMap.end() )
      continue;

    Package* pkgobj = it->second;
    if ( pkgobj->dependencies().empty() )
    {
      // only use if no dependencies in Pkgfile
      pkgobj->setDependencies( second );
    }
  }
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
