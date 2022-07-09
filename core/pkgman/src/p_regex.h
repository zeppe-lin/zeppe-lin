//! \file      p_regex.h
//! \brief     RegEx Definition

#pragma once

#include <string>

#include <regex.h>

using namespace std;

//! \class  RegEx
//! \brief  Regular Expression Matcher
class RegEx
{
public:
  //! \brief   Construct a RegEx object which can be used it's
  //!          \a match() method with different input strings
  //!
  //! \param   pattern        a regular expression pattern
  //! \param   caseSensitive  perform case-sensitive matching if true
  RegEx( const string& pattern, bool caseSensitive=false );

  //! Destroy the RegEx object
  ~RegEx();

  //! \brief   Whether a \a input string contains a \a RegEx pattern
  //!
  //! \param   input  the string for pattern matching
  //!
  //! \return  \a true if match, \a false otherwise
  bool match( const string& input );

  //! \brief   Whether a string contains a given pattern
  //!
  //! \param   pattern        a regular expression pattern
  //! \param   input          the string for pattern matching
  //! \param   caseSensitive  perform case-sensitive matching if true
  //!
  //! \return  \a true if match, \a false otherwise
  static bool match( const string&  pattern,
                     const string&  input,
                     bool           caseSensitive=false );

private:
  //! Save the pattern for searching with different input strings
  regex_t m_pattern;

  //! Regex compilation return value
  bool m_validPattern;
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
