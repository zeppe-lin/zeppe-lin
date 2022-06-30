//! \file      locker.cpp
//! \brief     Locker Implementation

#include <algorithm>
#include <fstream>

#include "locker.h"

Locker::Locker( const string& root ):
  m_openFailed( false ),
  m_root( root )
{
  ifstream file( m_root + _PATH_LOCKDB );
  if ( ! file.is_open() )
  {
    m_openFailed = true;
    return;
  }

  string line;
  while ( getline( file, line ) )
    if ( line.size() )
      m_packages.push_back( line );

  file.close();
}

bool Locker::openFailed() const
{
  return m_openFailed;
}

bool Locker::lock( const pkgname_t& package )
{
  if ( isLocked( package ) )
    return false; // already locked

  m_packages.push_back( package );
  return true;
}

bool Locker::unlock( const pkgname_t& package )
{
  auto it = find( m_packages.begin(), m_packages.end(), package );
  if ( it != m_packages.end() )
  {
    m_packages.erase( it );
    return true;
  }

  return false;
}

bool Locker::store()
{
  ofstream file( m_root + _PATH_LOCKDB );
  if ( ! file.is_open() )
    return false;

  for ( const auto& package: m_packages )
    file << package << endl;

  file.close();
  return true;
}

bool Locker::isLocked( const pkgname_t& package ) const
{
  auto it = find( m_packages.begin(), m_packages.end(), package );
  if ( it != m_packages.end() )
    return true;

  return false;
}

const vector< pkgname_t >& Locker::lockedPackages() const
{
  return m_packages;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
