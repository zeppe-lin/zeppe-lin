//! \file      pkgdb.h
//! \brief     PkgDB Definition

#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "package.h"
#include "pathnames.h"

using namespace std;

//! \class  PkgDB
//! \brief  The Representation of zeppelin's package database of installed
//!         packages
class PkgDB
{
public:
  //! \brief   Create a PkgDB object
  //!
  //! \param   root  use a different root directory
  PkgDB( const string& root="" );

  //! \brief   Check whether a package is installed
  //!
  //! \param   name  the package name
  //!
  //! \return  whether package \a name is installed
  bool isInstalled( const pkgname_t& name ) const;

  //! \brief   Get the package version and release
  //!
  //! \param   name  the package name
  //!
  //! \return  a package's version and release or an empty string
  //!          if not found
  string getVersionRelease( const pkgname_t& name ) const;

  //! \brief   Get all installed packages
  //!
  //! \return  a map of installed packages, where
  //!          pair.first   is the package name,
  //!          pair.second  is the version-release string
  const map< pkgname_t, pkgver_t >& installedPackages();

  //! \brief   Search packages for a match of \a pattern in name
  //!
  //! \note    The name can contain shell wildcards or regex
  //!
  //! \param   pattern   the pattern to be found
  //! \param   target    save matching result into the target, where
  //!                    pair.first   is the package name,
  //!                    pair.second  is the version-release string
  //! \param   useRegex  interpret the \a pattern as regular expression
  void getMatchingPackages( const string&                pattern,
                            map< pkgname_t, pkgver_t >&  target,
                            bool                         useRegex )
    const;

private:
  //! Load the package db
  bool load() const;

  //! Don't load the package db twice
  mutable bool m_isLoaded;

  //! All installed packages
  mutable map< pkgname_t, pkgver_t > m_packages;

  //! For a different than "/" root directory
  string m_root;
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
