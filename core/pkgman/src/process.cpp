//! \file       process.cpp
//! \brief      Process Implementation

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <list>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process.h"
#include "helpers.h"

using namespace std;
using namespace StringHelper;

Process::Process( const string&  app,
                  const string&  arguments,
                  int            logfd,
                  bool           log2stdout ):
  m_app( app ),
  m_arguments( arguments ),
  m_logfd( logfd ),
  m_log2stdout( log2stdout )
{
}

int Process::execute()
{
  list< string > args;
  split( m_arguments, ' ', args, /*startPos*/ 0, /*useEmpty*/ false );

  const size_t argc = 1 + args.size() + 1; // app, args, NULL

  char** argv = new char*[ argc ];

  size_t i = 0;
  argv[ i ] = const_cast< char* >( m_app.c_str() );
  for ( const auto& arg: args )
  {
    ++i;
    argv[ i ] = const_cast< char* >( arg.c_str() );
  }

  ++i;
  assert( i + 1 == argc );
  argv[ i ] = NULL;

  int status = m_logfd > 0 ? execLog( argc, argv )
                           : exec( argc, argv );

  delete [] argv;

  return status / 256;
}

int Process::executeShell( const char *shell )
{
  int status = m_logfd > 0 ? execShellLog( shell )
                           : execShell( shell );
  return status / 256;
}

int Process::exec( const size_t argc, char** argv )
{
  int status = 0;
  pid_t pid = fork();

  if ( pid == 0 )     // child process
  {
    execv( m_app.c_str(), argv );
    _exit( EXIT_FAILURE );
  }
  else if ( pid < 0 ) // fork failed
    status = -1;
  else                // parent process
  {
    if ( waitpid( pid, &status, 0 ) != pid )
      status = -1;
  }

  return status;
}

int Process::execLog( const size_t argc, char** argv )
{
  int status = 0;
  int fdpipe[ 2 ];

  pipe( fdpipe );

  pid_t pid = fork();
  if ( pid == 0 )     // child process
  {
    close( fdpipe[ 0 ] );

    dup2( fdpipe[ 1 ], STDOUT_FILENO );
    dup2( fdpipe[ 1 ], STDERR_FILENO );

    execv( m_app.c_str(), argv );
    exit( EXIT_FAILURE );
  }
  else if ( pid < 0 ) // fork failed
    status = -1;
  else                // parent process
  {
    // parent process
    close( fdpipe[ 1 ] );

    char readbuf[ 1024 ];
    int  wpval;

    while ( ( wpval = waitpid( pid, &status, WNOHANG ) ) == 0 )
    {
      ssize_t bytes;
      while ( ( bytes =
                  read( fdpipe[ 0 ], readbuf, sizeof( readbuf ) - 1 ) )
              > 0 )
      {
        readbuf[ bytes ] = 0;
        if ( m_log2stdout )
        {
          printf( "%s", readbuf );
          fflush( stdout );
          fflush( stderr );
        }
        write( m_logfd, readbuf, bytes );
      }
    }
    close( fdpipe[ 0 ] );

    if ( wpval != pid )
      status = -1;
  }

  return status;
}

int Process::execShell( const char* shell )
{
  int status = 0;
  pid_t pid = fork();

  if ( pid == 0 )     // child process
  {
    execl( shell, shell, "-c",
          ( m_app + " " + m_arguments ).c_str(), NULL );
    _exit( EXIT_FAILURE );
  }
  else if ( pid < 0 ) // fork failed
    status = -1;
  else                // parent process
  {
    if ( waitpid( pid, &status, 0 ) != pid )
      status = -1;
  }

  return status;
}

int Process::execShellLog( const char* shell )
{
  int status = 0;
  int fdpipe[ 2 ];

  pipe( fdpipe );

  pid_t pid = fork();
  if ( pid == 0 )     // child process
  {
    close( fdpipe[ 0 ] );

    dup2( fdpipe[ 1 ], STDOUT_FILENO );
    dup2( fdpipe[ 1 ], STDERR_FILENO );

    execl( shell, shell, "-x", "-c",
          ( m_app + " " + m_arguments ).c_str(), NULL );
    _exit( EXIT_FAILURE );
  }
  else if ( pid < 0 ) // fork failed
    status = -1;
  else                // parent process
  {
    close( fdpipe[ 1 ] );

    char readbuf[ 1024 ];
    int  wpval;

    while ( ( wpval = waitpid( pid, &status, WNOHANG ) ) == 0 )
    {
      ssize_t bytes;
      while ( ( bytes =
                  read( fdpipe[ 0 ], readbuf, sizeof( readbuf ) - 1 ) )
              > 0 )
      {
        readbuf[ bytes ] = 0;
        if ( m_log2stdout )
        {
          printf( "%s", readbuf );
          fflush( stdout );
          fflush( stderr );
        }
        write( m_logfd, readbuf, bytes );
      }
    }
    close( fdpipe[ 0 ] );

    if ( wpval != pid )
      status = -1;
  }

  return status;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
