//! \file      locker.h
//! \brief     Locker Definition

#pragma once

#include <string>
#include <vector>

#include "package.h"
#include "pathnames.h"

using namespace std;

//! \class  Locker
//! \brief  Lock the packages to skip updates while performing a system
//!         update
//!
//! Then the locked packages will:
//! - marked in 'pkgman diff \--all' as 'locked'
//! - not shown without '\--all' in 'pkgman diff'
//! - not updated in 'pkgman sysup'
//!
//! \warning REMEMBER TO CALL store!
class Locker
{
public:
  //! \brief   Open a locker file and read the locked packages into
  //!          \a m_packages
  Locker( const string& root );

  //! \brief   Check if opening failed
  //!
  //! \return  \a true if so, \a false otherwise
  bool openFailed() const;

  //! \brief   Lock the package
  //!
  //! \param   package  the package name
  //!
  //! \return  \a true if locking worked, \a false if already locked
  bool lock( const pkgname_t& package );

  //! \brief   Unlock the package
  //!
  //! \param   package  the package name
  //!
  //! \return  \a true if it could be unlocked,
  //!          \a false if it wasn't locked
  bool unlock( const pkgname_t& package );

  //! \brief   Write the changes to locker file
  //!
  //! \return  \a true if ok, \a false if locker file could not be
  //!          opened for writing
  bool store();

  //! \brief   Check if the \a package is locked
  //!
  //! \param   package  the package name
  //!
  //! \return  \a true if locked, \a false otherwise
  bool isLocked( const pkgname_t& package ) const;

  //! \brief   Get all locked packages
  //!
  //! \return  the vector of names of all locked packages
  const vector< pkgname_t >& lockedPackages() const;

private:
  //! The vector of names of all locked packages
  vector< pkgname_t > m_packages;

  //! true if lock file opening error
  bool m_openFailed;

  string m_root;
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
