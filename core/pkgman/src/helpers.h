//! \file      helpers.h
//! \brief     Helper Functions Definition

#pragma once

#include <map>
#include <list>
#include <string>

using namespace std;

// forward declaration
namespace ListHelper {};

//! \namespace  StringHelper
//! \brief      A generic place with string functions
namespace StringHelper
{
  // forward declaration
  template< class T > void split( const string&  str,
                                  char           del,
                                  T&             target,
                                  int            startPos=0,
                                  bool           useEmpty=true );

  //! \brief   Get the value after the first occurrence of \a del
  //!
  //! \param   str  the string to be searched
  //! \param   del  the delimiter char
  //!
  //! \return  the string from \a del position + 1 to the end of \a str
  string getValue( const string& str, char del );

  //! \brief   Get the value before the first occurrence of \a del
  //!
  //! \param   str  the string to be searched
  //! \param   del  the delimiter char
  //!
  //! \return  the string from 0 to the \a del position of \a str
  string getValueBefore( const string& str, char del );

  //! \brief   Strip whitespace in the beginning and end of string \a str
  //!
  //! \param   str  the string to be stripped
  //!
  //! \return  a stripped string
  string stripWhiteSpace( const string& str );

  //! \brief   Case sensitive implementation of startsWith()
  //!
  //! \param   a  the string to inspect
  //! \param   b  the string to search for
  //!
  //! \return \a true if the string \a a starts with \a b,
  //!         \a false otherwise
  bool startsWith( const string& a, const string& b );

  //! \brief   Case insensitive implementation of startsWith()
  //!
  //! \param   a  the string to inspect
  //! \param   b  the string to search for
  //!
  //! \return  \a true if the string \a a starts with \a b,
  //!          \a false otherwise
  bool startsWithNoCase( const string& a, const string& b );

  //! \brief   Case sensitive implementation of endsWith()
  //!
  //! \param   a  the string to inspect
  //! \param   b  the string to search for
  //!
  //! \return  \a true if the string \a a ends with \a b,
  //!          \a false otherwise
  bool endsWith( const string& a, const string& b );

  //! \brief   Case insensitive implementation of endsWith()
  //!
  //! \param   a  the string to inspect
  //! \param   b  the string to search for
  //!
  //! \return  \a true if the string \a a ends with \a b,
  //!          \a false otherwise
  bool endsWithNoCase( const string& a, const string& b );

  //! \brief   Convert the string into a lowercase representation
  //!
  //! \param   str  the string to be converted
  //!
  //! \return  a lowercase representation of \a str
  string toLowerCase( const string& str );

  //! \brief   Convert a string into a uppercase representation
  //!
  //! \param   str the string to be converted
  //!
  //! \return  an uppercase representation of \a str
  string toUpperCase( const string& str );

  //! \brief   Replace all occurrences of \a oldStr in \a in
  //!          with \a newStr
  //!
  //! \param   in      the string to modify
  //! \param   oldStr  the string to replace
  //! \param   newStr  the replacement string
  //!
  //! \return  a replaced representation of \a in string
  string replaceAll( string& in,
                     const string& oldStr,
                     const string& newStr );

  //! \brief   Metafunction to split a string into parts
  //!
  //! \tparam  str       the string to split
  //! \tparam  del       delimiter
  //! \tparam  target    sequence container that will contain the result
  //! \tparam  startPos  position to start at
  //! \tparam  useEmpty  include empty (whitespace only) results into
  //!                    the \a target
  template< class T >
    void split( const string&  str,
                char           del,
                T&             target,
                int            startPos,
                bool           useEmpty )
    {
      string line = str;

      string::size_type pos;
      int offset = startPos;
      while ( ( pos = line.find( del, offset ) ) != string::npos )
      {
        offset = 0;

        string val = line.substr( 0, pos );
        if ( useEmpty || stripWhiteSpace( val ).size() )
          target.push_back( val );

        line.erase( 0, pos + 1 );
      }

      if ( line.size() )
        target.push_back( line );
    }
}; // namespace StringHelper


//! \namespace  ListHelper
//! \brief      A generic place with list functions
namespace ListHelper
{
  //! \brief   Search if the list container contains an element
  //!
  //! \tparam  target   the list to inspect
  //! \tparam  element  the element to search for
  //!
  //! \return  \a true if contains, \a false otherwise
  template< typename T >
    bool contains( const list< T >& target, const T element )
    {
      auto it = find( target.begin(), target.end(), element );
      return it != target.end();
    }

  //! \brief   Search if the map container contains an element
  //!
  //! \tparam  target   the map to inspect
  //! \tparam  element  the element to search for
  //!
  //! \return  \a true if contains, \a false otherwise
  template< typename T >
    bool contains( const map< T, T >& target, const string& element )
    {
      return target.find( element ) != target.end();
    }

  //! \brief   Search if the string container contains an element
  //!
  //! \tparam  target   search in
  //! \tparam  element  the element
  //!
  //! \return  \a true if contains, \a false otherwise
  template< typename T1, typename T2 >
    bool contains( const T1& target, const T2& element )
    {
      if constexpr ( is_same< T1, T2 >::value )
        return target.find( element ) != string::npos;

      string s( element );
      return target.find( s ) != string::npos;
    }
}; // namespace ListHelper

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
