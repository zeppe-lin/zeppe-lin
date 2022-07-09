//! \file       main.cpp
//! \brief      Command-line utility of \a pkgman

#include <cstdlib>
#include <iostream>

#include <signal.h>

#include "argparser.h"
#include "process.h"
#include "pkgman.h"
#include "signaldispatcher.h"

using namespace std;

static void inline die( const string& message, int rc=EXIT_FAILURE )
{
  cerr << message << endl;
  exit( rc );
}

static void inline dieman( const string& cmd )
{
  Process man( _PATH_MAN_BIN, cmd, /*logfd*/ 0, /*log2stdout*/false );
  exit( man.execute() );
}

int main( int argc, char** argv )
{
  ArgParser parser( argc, argv );
  if ( ! parser.parse() )
  {
    if ( parser.unknownOption().size() )
      cerr << "pkgman: unknown option: " << parser.unknownOption() << endl;
    else if ( ! parser.isCommandGiven() )
    {
      if ( parser.commandName().empty() )
      {
        if ( parser.isVersion() )
          die( "pkgman " VERSION, EXIT_SUCCESS );
        else if ( parser.isHelp() )
          dieman( "pkgman" );
        else
          die( "pkgman: no command given" );
      }
      else
        die( "pkgman: unknown command: " + parser.commandName() );
    }
  }

  if ( parser.isVersion() )
    die( "pkgman " VERSION, EXIT_SUCCESS );

  if ( parser.isHelp() )
    dieman( "pkgman-" + parser.commandName() );

  if ( parser.verbose() > 2 )
    die( "pkgman: can't specify both -v and -vv" );

   if ( parser.isTest() )
    cout << "\n*** TEST MODE ***\n";

  Pkgman pkgman( &parser );
  //if ( pkgman.returnValue() == -1 )
  //  exit( EXIT_FAILURE );

  signal( SIGHUP,  SignalDispatcher::dispatch );
  signal( SIGINT,  SignalDispatcher::dispatch );
  signal( SIGQUIT, SignalDispatcher::dispatch );
  signal( SIGILL,  SignalDispatcher::dispatch );

  SignalDispatcher::instance()->registerHandler( &pkgman, SIGINT  );
  SignalDispatcher::instance()->registerHandler( &pkgman, SIGHUP  );
  SignalDispatcher::instance()->registerHandler( &pkgman, SIGQUIT );
  SignalDispatcher::instance()->registerHandler( &pkgman, SIGILL  );

  // some useful lambdas to avoid boilerplate

  auto argify = []( size_t count )
  {
    return  to_string( count ) + ( count > 1 ? " arguments" : " argument" );
  };

  auto assertMinArgCount = [ &parser, &argify ]( size_t count )
  {
    if ( parser.cmdArgs().size() < count )
      die( "pkgman: " + parser.commandName() + " takes at least " + argify( count ) );
  };

  auto assertMaxArgCount = [ &parser, &argify ]( size_t count )
  {
    if ( parser.cmdArgs().size() > count )
      die( "pkgman: " + parser.commandName() + " takes at most "  + argify( count ) );
  };

  auto assertExactArgCount = [ &parser, &argify ]( size_t count )
  {
    if ( parser.cmdArgs().size() != count )
      die( "pkgman: " + parser.commandName() + " takes exactly "  + argify( count ) );
  };

  //
  // Commands handling
  //

  switch ( parser.commandID() )
  {
    //
    // Informational
    //

    case ArgParser::DUMPCONFIG:
      pkgman.dumpConfig();
      break;

    case ArgParser::LIST:
      pkgman.listPackages();
      break;

    case ArgParser::LIST_DUP:
      assertMaxArgCount( 1 );
      pkgman.listShadowed();
      break;

    case ArgParser::LIST_NODEPENDENTS:
      pkgman.listNodependents();
      break;

    case ArgParser::LIST_ORPHANS:
      pkgman.listOrphans();
      break;

    case ArgParser::LIST_LOCKED:
      pkgman.listLocked();
      break;

    case ArgParser::PRINTF:
      assertExactArgCount( 1 );
      pkgman.printf();
      break;

    case ArgParser::INFO:
      assertExactArgCount( 1 );
      pkgman.printInfo();
      break;

    case ArgParser::README:
      assertExactArgCount( 1 );
      pkgman.printReadme();
      break;

    case ArgParser::PATH:
      assertExactArgCount( 1 );
      pkgman.printPath();
      break;

    case ArgParser::ISINST:
      assertMinArgCount( 1 );
      pkgman.printIsInstalled();
      break;

    case ArgParser::CURRENT:
      assertExactArgCount( 1 );
      pkgman.printCurrentVersion();
      break;

    //
    // Differences, Check for updates
    //

    case ArgParser::DIFF:
      pkgman.printDiff();
      break;

    //
    // Dependencies
    //

    case ArgParser::MDEP:
      pkgman.printMissingDep();
      break;

    case ArgParser::DEP:
      assertExactArgCount( 1 );
      pkgman.printDep();
      break;

    case ArgParser::RDEP:
      assertExactArgCount( 1 );
      pkgman.printRevDep();
      break;

    //
    // Searching
    //

    case ArgParser::SEARCH:
      assertExactArgCount( 1 );
      pkgman.psearch();
      break;

    case ArgParser::DSEARCH:
      assertExactArgCount( 1 );
      pkgman.psearch( /*desc*/ true );
      break;

    case ArgParser::FSEARCH:
      assertMinArgCount( 1 );
      pkgman.fsearch();
      break;

    //
    // Install, Update, Remove
    //

    case ArgParser::INSTALL:
      assertMinArgCount( 1 );
      pkgman.install( Transaction::INSTALL );
      break;

    case ArgParser::UPDATE:
      assertMinArgCount( 1 );
      pkgman.install( Transaction::UPDATE );
      break;

    case ArgParser::REMOVE:
      assertMinArgCount( 1 );
      pkgman.remove();
      break;

    //
    // System update
    //

    case ArgParser::SYSUP:
      pkgman.sysup();
      break;

    case ArgParser::LOCK:
      assertMinArgCount( 1 );   // package name
      pkgman.setLock( /*lock*/ true );
      break;

    case ArgParser::UNLOCK:
      assertMinArgCount( 1 );
      pkgman.setLock( /*lock*/ false );
      break;

    //
    // File operations
    //

    case ArgParser::LS:
      assertExactArgCount( 1 ); // package name
      pkgman.ls();
      break;

    case ArgParser::CAT:
      assertMinArgCount( 1 );   // package name
      assertMaxArgCount( 2 );   // optional file
      pkgman.cat();
      break;

    case ArgParser::EDIT:
      assertMinArgCount( 1 );   // package name
      assertMaxArgCount( 2 );   // optional file
      pkgman.edit();
      break;

    // // // //

    default:
      cerr << "pkgman: unknown command" << endl;
      exit( 1 );
  } // end switch

  if ( parser.isTest() )
    cout << "\n*** TEST MODE END ***\n";

  return pkgman.returnValue();
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
