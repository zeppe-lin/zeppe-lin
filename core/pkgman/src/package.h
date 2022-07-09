//! \file       package.h
//! \brief      Package and PackageData Definition

#pragma once

#include <string>

using namespace std;

// forward declaration
struct PackageData;

// useful aliases
typedef string pkgname_t;   //!<  Package name
typedef string pkgver_t;    //!<  Package version-release

//! \class  Package
//! \brief  The Representation of a Package from the GNU/Zeppe-Lin
//!         packages sources tree
class Package
{
public:
  //! \brief   Create a package, which is not yet fully initialized
  //!
  //! \param   name  the package name
  //! \param   path  the package path
  //!
  //! \note    This is interesting in combination with the lazy
  //!          initialization
  Package( const string& name, const string& path );

  //! \brief   Create a fully initialized package
  //!
  //! \param   name            the package name
  //! \param   path            the package path
  //! \param   version         the package version
  //! \param   release         the package release number
  //! \param   description     the package description
  //! \param   dependencies    the package dependencies
  //! \param   url             the sources' location used to build
  //!                          this package
  //! \param   packager        the packager of the package
  //! \param   maintainer      the maintainer of the package
  //! \param   hasReadme       whether package has a README file
  //! \param   hasPreInstall   whether package has a pre-install script
  //! \param   hasPostInstall  whether package has a post-install script
  //! \param   hasPreRemove    whether package has a pre-remove script
  //! \param   hasPostRemove   whether package has a post-remove script
  Package( const string&  name,
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
           const string&  hasPostRemove);

  //! destroy the package
  ~Package();

  //! \brief   Get the package name
  //!
  //! \return  the name of this package
  const string& name() const;

  //! \brief   Get the package path in the packages sources tree
  //!
  //! \return  the path to this package
  const string& path() const;

  //! \brief   Get the full path to this package or this package's
  //!          \a file in the packages sources tree
  //!
  //! \param   file  the optional file name to be added at the end of
  //!          package path
  //!
  //! \return  the full path to this package or package file,
  //!          in case of specified \a file argument
  const string fullpath( const string& file="" ) const;

  //! \brief   Get package version (without release number)
  //!
  //! \return  the version of this package
  const string& version() const;

  //! \brief   Get package release number
  //!
  //! \return  the release number of this package
  const string& release() const;

  //! \brief   Get package version-release
  //!
  //! \return  a typically formatted \b version-release string
  string version_release() const;

  //! \brief   Get package description
  //!
  //! \return  the description field of this package
  const string& description() const;

  //! \brief   Get package dependencies
  //!
  //! \return  the dependencies (comma-split line) of this package
  const string& dependencies() const;

  //! \brief   Get the sources' location used to build this package
  //!
  //! \return  the sources' location for this package
  const string& url() const;

  //! \brief   Get the packager of this package
  //!
  //! \return  the packager of this package
  const string& packager() const;

  //! \brief   Get the package maintainer
  //!
  //! \return  the maintainer of this package
  const string& maintainer() const;

  //! \brief   Whether or not this package has a readme file
  //!
  //! \return  \a true if so, \a false otherwise
  bool hasReadme() const;

  //! \brief   Whether or not this package has a pre-install script
  //!
  //! \return  \a true if so, \a false otherwise
  bool hasPreInstall() const;

  //! \brief   Whether or not this package has a post-install script
  //!
  //! \return  \a true if so, \a false otherwise
  bool hasPostInstall() const;

  //! \brief   Whether or not this package has a pre-remove script
  //!
  //! \return  \a true if so, \a false otherwise
  bool hasPreRemove() const;

  //! \brief   Whether or not this package has a post-remove script
  //!
  //! \return  \a true if so, \a false otherwise
  bool hasPostRemove() const;

  //! \brief   Set dependencies for this package
  //!
  //! \param   dependencies  a comma-split packages
  void setDependencies( const string& dependencies );

private:
  //! The config file with custom specified package dependencies
  static const string CUSTOM_DEPS_FILE;

  //! Load necessary package data from Pkgfile and package source's
  //! directory
  void load() const;

  //! Rewrite Pkgfile's dependencies to ones written in the config file
  void loadConfigDepends() const;

  //! \brief   Expand shell commands
  //!
  //! \param   input     the string with shell commands that will be
  //!                    modified
  //! \param   timeNow   time_t structure
  //! \param   unameBuf  utsname structure
  //!
  //! \note    Currently are supported only date and uname
  static void expandShellCommands( string&               input,
                                   const time_t&         timeNow,
                                   const struct utsname  unameBuf );

  //! this package data fields
  mutable PackageData* m_data;

  //! don't load() data twice
  mutable bool m_loaded;
}; // Package


//! \struct  PackageData
//! \brief   The package data fields
struct PackageData
{
  PackageData( const string&  name_,
               const string&  path_,
               const string&  version_="",
               const string&  release_="",
               const string&  description_="",
               const string&  dependencies_="",
               const string&  url_="",
               const string&  packager="",
               const string&  maintainer="",
               const string&  hasReadme_="",
               const string&  hasPreInstall_="",
               const string&  hasPostInstall_="",
               const string&  hasPreRemove_="",
               const string&  hasPostRemove_="" );

  //! the name of this package
  string name;

  //! the path to this package
  string path;

  //! the version of this package
  string version;

  //! the release number of this package
  string release;

  //! the typically formatted version-release string of this package
  string version_release;

  //! the description of this package
  string description;

  //! the dependencies of this package
  string depends;

  //! the upstream URL location of this package
  string url;

  //! the packager of this package
  string packager;

  //! the maintainer of this package
  string maintainer;

  //! whether or not this package has a readme file
  bool hasReadme;

  //! whether or not this package has a pre-install script
  bool hasPreInstall;

  //! whether or not this package has a post-install script
  bool hasPostInstall;

  //! whether or not this package has a pre-remove script
  bool hasPreRemove;

  //! whether or not this package has a post-remove script
  bool hasPostRemove;

  //! generate the \a version_release string
  void generateVersionReleaseString();
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
