//! \file      argparser.h
//! \brief     ArgParser class definition

#pragma once

#include <list>
#include <string>

using namespace std;

//! \class   ArgParser
//! \brief   Argument Parser
//!
//! This is command-line argument parser for pkgman.
class ArgParser
{
public:

  //! TODO
  enum Command_ID {
    // Informational
    DUMPCONFIG,
    LIST,
    LIST_DUP,
    LIST_NODEPENDENTS,
    LIST_ORPHANS,
    LIST_LOCKED,
    PRINTF,
    INFO,
    README,
    PATH,
    ISINST,
    CURRENT,
    // Differences / Check for updates
    DIFF,
    // Dependencies
    MDEP,
    DEP,
    RDEP,
    // Searching
    SEARCH,
    DSEARCH,
    FSEARCH,
    // Install / Update / Remove
    INSTALL,
    UPDATE,
    REMOVE,
    // System update
    SYSUP,
    LOCK,
    UNLOCK,
    // File operations
    LS,
    CAT,
    EDIT,
  };

  //! \brief   Construct an ArgParser object
  //!
  //! \param   argc  argument count
  //! \param   argv  argument vector
  ArgParser( int argc, char** argv );

  //! \brief   Parse the arguments
  bool parse();

  //! \brief   Whether command is specified
  bool isCommandGiven() const;

  //! TODO
  size_t commandID() const;

  //! \brief   Whether \-V or \--version command-line option is specified
  bool isVersion() const;

  //! \brief   Whether \--help command-line option is specified
  bool isHelp() const;

  //! \brief   Whether \--force command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool isForced() const;

  //! \brief   Whether \--test command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool isTest() const;

  //! \brief   Whether \--config="..." command-line option is specified
  //!
  //! \return  \a true if an alternate configuration file is given,
  //!          \a false otherwise
  bool isAlternateConfigGiven() const;

  //! \brief   Whether \--log command-line option has been specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool writeLog() const;

  //! \brief   Whether \--filter="..." command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool hasFilter() const;

  //! \brief   Whether \--no-std-config command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool noStdConfig() const;

  //! \brief   Whether \--deps command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool deps() const;

  //! \brief   Whether \--nodeps command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool nodeps() const;

  //! \brief   Whether \--all command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool all() const;

  //! \brief   Whether \--full command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool full() const;

  //! \brief   Whether \--path command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool printPath() const;

  //! \brief   Whether \--pre-install command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool execPreInstall() const;

  //! \brief   Whether \--post-install command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool execPostInstall() const;

  //! \brief   Whether \--pre-remove command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool execPreRemove() const;

  //! \brief   Whether \--post-remove command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool execPostRemove() const;

  //! \brief   Whether \--regex command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool useRegex() const;

  //! \brief   Whether \--recursive command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool recursive() const;

  //! \brief   Whether \--tree command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool printTree() const;

  //! \brief   Whether \--group command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool group() const;

  //! \brief   Whether \--depsort command-line option is specified
  //!
  //! \return  \a true if so, \a false otherwise
  bool depSort() const;

  //! \brief   The name of the alternative configuration file
  //!
  //! \return  the \--config="..." option argument
  const string& alternateConfigFile() const;

  //! \brief   Additional arguments to \b makecommand
  //!
  //! \return  the \--margs="..." option argument
  const string& pkgmkArgs() const;

  //! \brief   Additional arguments to \b addcommand
  //!
  //! \return  the \--aargs="..." option argument
  const string& pkgaddArgs() const;

  //! \brief   Additional arguments to \b removecommand
  //!
  //! \return  the \--rargs="..." option argument
  const string& pkgrmArgs() const;

  //! \brief   Sort by ...
  //!
  //! \return  the \--sort="..." option argument
  const string& sortArgs() const;

  //! \brief   Filter by ...
  //!
  //! \return  the \--filter="..." option argument
  const string& filter() const;

  //! \brief   Alternative root directory
  //!
  //! \return  the \--root="..." option argument
  const string& root() const;

  //! \brief   Ignore packages sources
  //!
  //! \return  the \--ignore="..." option argument
  const string& ignore() const;

  //! \brief   Command name
  //!
  //! \return  what command was given
  const string& commandName() const;

  //! \brief   Unknown option
  //!
  //! \return  what exactly unknown option was given
  const string& unknownOption() const;

  //! \brief   Other command-line arguments
  //!
  //! \return  a list of arguments not processed by ArgParser
  const list< char* >& cmdArgs() const;

  //! \brief   Verbosity level
  //!
  //! \return  the level of verbose: \-v -> 1, \-vv -> 2
  int verbose() const;

  //! Config argument type
  enum configArg_t
  {
    CONFIG_SET,       //!<  \--config-set="..."     argument type
    CONFIG_APPEND,    //!<  \--config-append="..."  argument type
    CONFIG_PREPEND    //!<  \--config-prepend="..." argument type
  };

  //! \brief   Config arguments
  //!
  //! \return  the list of given \--config-{set,append,prepend}="..."
  //!          arguments with corresponding config argument type
  const list< pair< char*, configArg_t > > configData() const;

private:

  //! TODO
  bool m_isCommandGiven;

  //! TODO
  size_t m_commandID;

  //! Command argument
  string m_commandName;

  //! Unknown option
  string m_unknownOption;

  //! \-h, \--help options
  bool m_Help;

  //! \-v, \--version
  bool m_Version;

  //! \--force option
  bool m_isForced;

  //! \--test option
  bool m_isTest;

  //! \--config="..." option
  bool m_isAlternateConfigGiven;

  //! \--filter="..." option
  bool m_hasFilter;

  //! \--no-std-config option
  bool m_noStdConfig;

  //! \--log option
  bool m_writeLog;

  //! \--deps option
  bool m_deps;

  //! \--all option
  bool m_all;

  //! \--full option
  bool m_full;

  //! \--path option
  bool m_printPath;

  //! \--pre-install option
  bool m_execPreInstall;

  //! \--post-install option
  bool m_execPostInstall;

  //! \--pre-remove option
  bool m_execPreRemove;

  //! \--post-remove option
  bool m_execPostRemove;

  //! \--regex option
  bool m_useRegex;

  //! \--recursive option
  bool m_recursive;

  //! \--tree option
  bool m_printTree;

  //! \--group option
  bool m_group;

  //! \--depsort option
  bool m_depSort;

  //! \--config="..." option argument
  string m_alternateConfigFile;

  //! \--margs="..." option argument
  string m_pkgmkArgs;

  //! \--aargs="..." option argument
  string m_pkgaddArgs;

  //! \--rargs="..." option argument
  string m_pkgrmArgs;

  //! \--sort="..." option argument
  string m_sortArgs;

  //! \--filter="..." option argument
  string m_filter;

  //! \--root="..." option argument
  string m_root;

  //! \--ignore="..." option argument
  string m_ignore;

  //! argument count
  int m_argc;

  //! argument vector
  char** m_argv;

  //! verbose argument (0: no verbose, 1: -v , 2: -vv)
  int m_verbose;

  //! a list of arguments not processed by ArgParser
  list< char* > m_cmdArgs;

  //! a list of given --config-{set,append,prepend}="..."
  //! arguments with corresponding config argument type
  list< pair< char*, configArg_t > > m_configData;
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
