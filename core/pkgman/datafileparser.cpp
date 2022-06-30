//! \file      datafileparser.cpp
//! \brief     DataFileParser Implementation

#include <fstream>

#include "datafileparser.h"
#include "helpers.h"

using namespace std;
using namespace StringHelper;

bool DataFileParser::parse( const string&           filename,
                            map< string, string >&  target )
{
  ifstream file( filename );
  if ( ! file.is_open() )
    return false;

  string line;
  while ( getline( file, line ) )
  {
    line = stripWhiteSpace( line );
    if ( line.empty() || line[ 0 ] == '#' )
      continue;

    if ( auto pos = line.find( ":" ); pos != string::npos )
    {
      string name = stripWhiteSpace( line.substr( 0, pos ) );
      string deps = stripWhiteSpace( line.substr( pos + 1 ) );
      deps = replaceAll( deps, "  ", " " );
      deps = replaceAll( deps, " ",  "," );
      deps = replaceAll( deps, ",,", "," );

      target[ name ] = deps;
    }
  }
  file.close();

  return true;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
