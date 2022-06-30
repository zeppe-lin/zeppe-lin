//! \file      p_regex.cpp
//! \brief     RegEx Implementation

#include <string>

#include "p_regex.h"

using namespace std;

RegEx::RegEx( const string& pattern, bool caseSensitive )
{
  int additionalFlags = 0;

  if ( ! caseSensitive )
    additionalFlags |= REG_ICASE;

  m_validPattern =
    regcomp( &m_pattern, pattern.c_str(),
             REG_EXTENDED | REG_NOSUB | additionalFlags ) == 0;
}

RegEx::~RegEx()
{
  regfree( &m_pattern );
}

bool RegEx::match( const string& input )
{
  if ( ! m_validPattern )
    return false;

  return regexec( &m_pattern, input.c_str(), 0, 0, 0 ) == 0;
}

bool RegEx::match( const string&  pattern,
                   const string&  input,
                   bool           caseSensitive )
{
  RegEx re( pattern, caseSensitive );
  return re.match( input );
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
