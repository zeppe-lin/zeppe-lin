//! \file      pkgdb.cpp
//! \brief     PkgDB Implementation

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>

#include <fnmatch.h>

#include "datafileparser.h"
#include "p_regex.h"
#include "pkgdb.h"
#include "helpers.h"

using namespace std;
using namespace StringHelper;

PkgDB::PkgDB( const string& root ):
  m_isLoaded( false ),
  m_root( root )
{
}

bool PkgDB::isInstalled( const pkgname_t& name ) const
{
  if ( ! load() )
    return false;

  return m_packages.find( name ) != m_packages.end();
}

string PkgDB::getVersionRelease( const pkgname_t& name ) const
{
  if ( ! load() )
    return "";

  const auto& pkg = m_packages.find( name );
  if ( pkg == m_packages.end() )
    return "";

  return pkg->second; /* version-release */
}

const map< pkgname_t, pkgver_t >& PkgDB::installedPackages() {
  load();
  return m_packages;
}

void
PkgDB::getMatchingPackages( const string&                pattern,
                            map< pkgname_t, pkgver_t >&  target,
                            bool                         useRegex )
  const
{
  if ( ! load() )
    return;

  if ( useRegex )
  {
    RegEx re( pattern );

    for ( const auto& [ name, version ]: m_packages )
      if ( re.match( name ) )
        target[ name ] = version;
  }
  else
  {
    for ( const auto& [ name, version ]: m_packages )
    {
      // jw: I assume fnmatch will be quite fast for "match all" (*),
      // so I didn't add a boolean to check for this explicitely
      if ( fnmatch( pattern.c_str(), name.c_str(), 0 ) == 0 )
        target[ name ] = version;
    }
  }
}

bool PkgDB::load() const
{
  if ( m_isLoaded )
    return true;

  string line;
  string name;
  bool emptyLine = true;
  bool nameRead = false;

  ifstream db( m_root + _PATH_PKGDB );
  if ( ! db.is_open() )
    return false;

  while ( db.good() )
  {
    getline( db, line );
    if ( emptyLine && line.size() )
    {
      name      = line;
      emptyLine = false;
      nameRead  = true;
    }
    else if ( nameRead )
    {
      if ( line.size() )
        m_packages[ name ] = line; // version

      nameRead = false;
    }
    if ( line.empty() )
      emptyLine = true;
  }
  db.close();

  return m_isLoaded = true;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
