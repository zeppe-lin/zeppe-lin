//! \file      pkgmksetting.h
//! \brief     Get settings from pkgmk configuration

#pragma once

#include <string>

#include "pathnames.h"

class ArgParser;

//! \class  PkgmkSetting
//! \brief  Get setting from pkgmk configuration.
class PkgmkSetting
{
public:
  //! \brief  Construct a PkgmkSetting object
  PkgmkSetting( const ArgParser* parser );

  //! \brief  Get setting from pkgmk(8)
  //!
  //! Get setting from \b /etc/pkgmk.conf file it exists, from
  //! -cf <file> specified as --margs='...' argument, or from
  //! \b /usr/sbin/pkgmk otherwise.
  //!
  //! \note   This is a helper around private \a getSettingFromFile().
  //!
  //! \param  setting   any string defined as "setting=value" in
  //!                   pkgmk configuration file
  //!
  //! \return the setting's value
  const string get( const string& setting );

private:

  //! Argument parser
  const ArgParser* m_parser;

  //! \brief  Get setting from specified file
  //!
  //! \param  setting   any string defined as "setting=value"
  //!                   in \a filename
  //! \param  filename  path to the settings file
  //!
  //! \return the setting's value
	string getSettingFromFile( const string& setting,
                             const string& filename );
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
