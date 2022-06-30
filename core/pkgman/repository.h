//! \file       repository.h
//! \brief      Repository class definition

#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>

#include "package.h"

using namespace std;

// TODO
typedef list< pair< string, string > > rootList_t;

//! \class  Repository
//! \brief  Repository of available packages sources
//!
//! The repository is an abstraction of the available packages sources
//! in the packages sources tree
class Repository
{
public:
  //! \brief   Create a repository
  //!
  //! \param   useRegex  interpret the search pattern in methods
  //!                    \a searchMatchingPackages and
  //!                    \a getMatchingPackages as regular expression
  Repository( bool useRegex );

  //! Destroy a repository
  ~Repository();

  //! \brief   Get the package
  //!
  //! \param   name the package name to be returned
  //!
  //! \return  a \a Package pointer for a package name or 0 if not found
  const Package*
  getPackage( const pkgname_t& name )
  const;

  //! \brief   Get all the packages available in the repository
  //!
  //! \return  a map of available packages, where
  //!          pair.first   is the package name,
  //!          pair.second  is \a Package pointer
  const map< pkgname_t, Package* >&
  packages()
  const;

  //! \brief   Get the sorted list of duplicate packages
  //!          in the repository
  //!
  //! \return  a list of duplicate packages in the repo,
  //!          wherein the pairs
  //!          .first   is the shadowed package source and
  //!          .second  is the package source which precedes over first
  const list< pair< Package*, Package* > >&
  shadowedPackages()
  const;

  //! \brief   Search packages for a match of \a pattern in name, and
  //!          description if \a searchDesc is \a true
  //!
  //! \note    The name can be interpreted as regex
  //!          if \a m_useRegex is set
  //!
  //! \note    Name searches can often done without opening the
  //!          Pkgfiles, but not description search.
  //!          Therefore, the later is much slower.
  //!
  //! \param   pattern     the pattern to be found
  //! \param   target      save matching packages into a \a target list
  //! \param   searchDesc  whether descriptions should be searched
  //!                      as well
  void
  searchMatchingPackages( const string&      pattern,
                          list< Package* >&  target,
                          bool               searchDesc )
  const;

  //! \brief   Search packages for a match of \a pattern in name
  //!
  //! \note    The name can contain shell wildcards or be interpreted
  //!          as regex if \a m_useRegex is set
  //!
  //! \param   pattern  the pattern to be found
  //! \param   target   save matching packages into a \a target list
  void
  getMatchingPackages( const string&      pattern,
                       list< Package* >&  target )
  const;

  //! \brief   Init repository by reading the directories passed
  //!
  //! \note    Doesn't search recursively, so if you want /dir and
  //!          /dir/subdir checked, you have to specify both
  //!
  //! \param   rootList       a list of directories to look for
  //!                         packages sources in
  //! \param   listDuplicate  whether duplicates should be registered
  //!                         (slower)
  void initFromFS( const rootList_t& rootList, bool listDuplicate );

  //! \brief   Create all components of the \a path which don't exist
  //!
  //! \param   path  the path to be created
  //!
  //! \return  \a true on success,
  //!          \a false mostly indicates permission problems
  static
  bool createOutputDir( const string& path );

  //! \brief   Add custom dependency for a package
  //!
  //! XXX Add more description
  void
  addDependencies( map< string, string >& deps );

private:
  //! \brief   The helper function for sorting the shadowed packages
  //!          by name
  //!
  //! \param   p1  the first pair of shadowed packages
  //! \param   p2  the second pair of shadowed packages
  //!
  //! \return  the comparison result of \code \a p1 < \a p2 \endcode
  static int
  compareShadowPair( pair< Package*, Package* >&  p1,
                     pair< Package*, Package* >&  p2 );

  //! Whether interpret the matching \a pattern as a regular expression
  //! \see \a searchMatchingPackages and \a getMatchingPackages
  bool m_useRegex;

  //! The list of duplicated packages in the repo
  list< pair< Package*, Package* > > m_shadowedPackages;

  //! The repository of all available packages
  map< pkgname_t, Package* > m_packageMap;
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
