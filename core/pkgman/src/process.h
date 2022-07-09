//! \file      process.h
//! \brief     Process Definition

#pragma once

#include <string>

using namespace std;

//! \class  Process
//! \brief  Execute the processes
class Process
{
public:
  //! \brief   Create a process
  //! \param   app         the application to execute
  //! \param   arguments   the arguments to be passed to \a app
  //! \param   logfd       log file descriptor
  //! \param   log2stdout  whether make logging to stdout
  Process( const string&  app,
           const string&  arguments,
           int            logfd=0,
           bool           log2stdout=false );

  //! \brief   Execute the process
  //!
  //! \return  the execution state of the application
  int execute();

  //! \brief   Execute the process using the shell
  //!
  //! \param   shell  path to the command interpreter
  //!
  //! \return  the execution state of the application
  int executeShell( const char* shell="/bin/sh" );

private:
  //! \brief   Execute the process
  //!
  //! \param   argc  argument count
  //! \param   argv  argument vector
  //!
  //! \return  the exit status of the application
  int exec( const size_t argc, char** argv );

  //! \brief   Execute the process and log the output to a file
  //!          descriptor
  //!
  //! \note    Will also make logging to stdout
  //!          if \a m_log2stdout is set
  //!
  //! \param   argc  argument count
  //! \param   argv  argument vector
  //!
  //! \return  the exit status of the application
  int execLog( const size_t argc, char** argv );

  //! \brief   Execute the process using the shell
  //!
  //! \param   shell  path to the command interpreter
  //!
  //! \return  the exit status of the application
  int execShell( const char* shell );

  //! \brief   Execute the process using the shell and log the output
  //!          to a file descriptor
  //!
  //! \note    Will also make logging to stdout
  //!          if \a m_log2stdout is set
  //!
  //! \param   shell  path to the command interpreter
  //!
  //! \return  the exit status of the application
  int execShellLog( const char* shell );

  //! Application to execute
  string m_app;

  //! Application arguments
  string m_arguments;

  //! The log file descriptor
  int m_logfd;

  //! Whether make logging to stdout
  bool m_log2stdout;
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
