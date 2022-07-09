//! \file      helpers.cpp
//! \brief     Helper Functions Implementation

#include <algorithm>

#include "helpers.h"

using namespace std;

string StringHelper::getValue( const string& str, char del )
{
  const auto& pos = str.find( del );
  if ( pos != string::npos && pos + 1 < str.size() )
    return str.substr( pos + 1 );
  return "";
}

string StringHelper::getValueBefore( const string& str, char del )
{
  const auto& pos = str.find( del );
  if ( pos != string::npos )
    return str.substr( 0, pos );
  return str;
}

string StringHelper::stripWhiteSpace( const string& str )
{
  if ( str.empty() )
    return str;

  ssize_t pos = 0;
  string line = str;
  ssize_t len = line.length();

  while ( pos < len && isspace( line[ pos ] ) )
    ++pos;

  line.erase( 0, pos );
  pos = line.length() - 1;

  while ( pos != -1 && isspace( line[ pos ] ) )
    --pos;

  if ( pos != -1 )
    line.erase( pos + 1 );

  return line;
}

bool StringHelper::startsWith( const string& a, const string& b )
{
  if ( b.size() > a.size() )
    return false;

  // std::string::find returns 0 if b is found at starting
  return a.find( b ) == 0;
}

bool StringHelper::startsWithNoCase( const string& a, const string& b )
{
  if ( b.size() > a.size() )
    return false;

  string x( toLowerCase( a ) ),
         y( toLowerCase( b ) );

  return x.find( y ) == 0;
}

bool StringHelper::endsWith( const string& a, const string& b )
{
  if ( b.size() > a.size() )
    return false;

  return equal( b.rbegin(), b.rend(), a.rbegin() );
}

bool StringHelper::endsWithNoCase( const string& a, const string& b )
{
  if ( b.size() > a.size() )
    return false;

  string x( toLowerCase( a ) ),
         y( toLowerCase( b ) );

  return equal( y.rbegin(), y.rend(), x.rbegin() );
}

string StringHelper::toLowerCase( const string& str )
{
  string p( str );
  transform( p.begin(), p.end(), p.begin(), ::tolower );

  return p;
}

string StringHelper::toUpperCase( const string& s )
{
  string p( s );
  transform( p.begin(), p.end(), p.begin(), ::toupper );

  return p;
}

string StringHelper::replaceAll( string&        in,
                                 const string&  oldStr,
                                 const string&  newStr )
{
  size_t pos;
  while ( ( pos = in.find( oldStr ) ) != string::npos )
    in = in.replace( pos, oldStr.length(), newStr );

  return in;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
