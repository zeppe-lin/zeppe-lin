//! \file      versioncomparator.cpp
//! \brief     VersionComparator namespace implementation
//!
//! \note      Get a test application with the following command:
//!            g++ -o vcomp -DTEST helpers.cpp versioncomparator.cpp
//!

#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <stdlib.h>

#include "helpers.h"
#include "versioncomparator.h"

using namespace std;
using namespace StringHelper;

namespace VersionComparator
{
  // jw: this is a one day hack;
  //     I'll refactor that to be nicer, clearer and hopefully faster.
  //     It should work pretty well though

  COMP_RESULT compareVersions( const string& v1, const string& v2 )
  {
    vector< string > blocks1;
    vector< string > blocks2;

    tokenizeIntoBlocks( v1, blocks1 );
    tokenizeIntoBlocks( v2, blocks2 );

    size_t blockLen = normalizeBlocksSize( blocks1, blocks2 );

    for ( size_t i = 0; i < blockLen; ++i )
    {
      vector< string > tokens1;
      vector< string > tokens2;

      split( blocks1[ i ], '.', tokens1 );
      split( blocks2[ i ], '.', tokens2 );

      size_t tokLen = normalizeBlocksSize( tokens1, tokens2 );

      for ( size_t j = 0; j < tokLen; ++j )
      {
        if ( tokens1[ j ] == tokens2[ j ] )
          continue;

        char* error1 = 0;
        char* error2 = 0;

        long vl1 = strtol( tokens1[ j ].c_str(), &error1, 10 );
        long vl2 = strtol( tokens2[ j ].c_str(), &error2, 10 );

        if ( *error1 != 0 || *error2 != 0 )
        {
          // subtokenize
          vector< string > subtokens1;
          vector< string > subtokens2;

          tokenizeMixedIntoBlocks( tokens1[ j ], subtokens1 );
          tokenizeMixedIntoBlocks( tokens2[ j ], subtokens2 );

          size_t subTokLen =
            normalizeBlocksSize( subtokens1, subtokens2 );

          for ( size_t k = 0; k < subTokLen; ++k )
          {
            long sl1 = strtol( subtokens1[ k ].c_str(), &error1, 10 );
            long sl2 = strtol( subtokens2[ k ].c_str(), &error2, 10 );

            if ( *error1 == 0 && *error2 == 0 )
            {
              if ( sl1 < sl2 )
                return LESS;
              else if ( sl1 > sl2 )
                return GREATER;
            }
            else
            {
              // string tokens
              if (   subtokens1[ k ][ 1 ] == 0
                  && subtokens2[ k ][ 1 ] == 0 )
              {
                if ( subtokens1[ k ][ 0 ] < subtokens2[ k ][ 0 ] )
                  return LESS;
                else if ( subtokens1[ k ][ 0 ] > subtokens2[ k ][ 0 ] )
                  return GREATER;
              }
              else
              {
                // smart guessing...
                // leaving out 'test', 'pre' and 'rc'
                static const string versions = "alpha beta gamma delta";

                string::size_type pos1 =
                  versions.find( subtokens1[ k ] );

                string::size_type pos2 =
                  versions.find( subtokens2[ k ] );

                if ( pos1 != string::npos && pos2 != string::npos )
                {
                  if ( pos1 < pos2 )
                    return LESS;
                  else if ( pos1 > pos2 )
                    return GREATER;
                }
              }

              if ( subtokens1[ k ] != subtokens2[ k ] )
                return UNDEFINED;
            }
          } // for k
        }
        else if ( vl1 < vl2 )
          return LESS;
        else if ( vl1 > vl2 )
          return GREATER;
      } // for j
    } // for i

    return EQUAL;
  }

  size_t
    normalizeBlocksSize( vector< string >&  v1,
                         vector< string >&  v2 )
  {
    size_t length = max( v1.size(), v2.size() );

    while ( v1.size() < length )
      v1.push_back( "-1" );

    while ( v2.size() < length )
      v2.push_back( "-1" );

    return length;
  }

  void
    tokenizeMixedIntoBlocks( const string&      s,
                             vector< string >&  tokens )
  {
    vector< bool > digitMask;
    for ( size_t i = 0; i < s.length(); ++i )
      digitMask.push_back( isdigit( s[ i ] ) );

    bool state = digitMask[ 0 ];
    string tok;
    tok = s[ 0 ];

    for ( size_t i = 1; i < digitMask.size(); ++i )
    {
      if ( digitMask[ i ] != state )
      {
        tokens.push_back( tok );
        tok = s[ i ];
        state = digitMask[ i ];
      }
      else
        tok += s[ i ];
    }

    if ( tok.length() > 0 )
      tokens.push_back( tok );
  }

  /*
    find last - (for release) -> version, release
    subdivide version in blocks, where a block is separated by
    any of the following: [-_]

    -> list of blocks, e.g.
    .  1.4.2-pre1-2 ->  [ (1.4.2) (-pre1) (2) ]
    .  1.4.2pre1-1  ->  [ (1.4.2pre1) (-1) ]
    .  1_2_2pre2-2  ->  [ (1) (2) (2pre2) (2)
  */
  void
    tokenizeIntoBlocks( const string&      version,
                        vector< string >&  blocks )
  {
    string v = version;
    v = replaceAll( v, "-", "_" );
    split( v, '_', blocks );
  }
} // namespace VersionComparator

#ifdef TEST

using namespace VersionComparator;

void check( const string&  v1,
            const string&  v2,
            COMP_RESULT    expected,
            bool           compare=true )
{
  auto result = compareVersions( v1, v2 );

  if ( compare )
    cout << ( result == expected ? "OK   " : "FAIL " );

  cout << v1 << " ";
  switch ( result )
  {
    case LESS:
      cout << "<";
      break;
    case GREATER:
      cout << ">";
      break;
    case EQUAL:
      cout << "=";
      break;
    case UNDEFINED:
      cout << "?";
      break;
  }
  cout << " " << v2 << endl;
}

int
main( int  argc, char**  argv )
{
  if ( argc < 3 )
  {
    check( "1",             "2",            LESS      );
    check( "1.1",           "1.2",          LESS      );
    check( "1.1pre1",       "1.1pre2",      LESS      );
    check( "1.1pre1",       "1.2pre1",      LESS      );
    check( "1.1-pre1",      "1.1-pre2",     LESS      );
    check( "1.1_2",         "1.1.2",        LESS      );
    check( "1.1",           "1.1",          EQUAL     );

    check( "1.0PR1",        "1.0PR1",       EQUAL     );
    check( "1.0PR1",        "1.0PR2",       LESS      );
    check( "1.0PR1",        "1.0RC1",       UNDEFINED );

    check( "1.2.3-2",       "1.2.3-1",      GREATER   );
    check( "1.0.0",         "0.9",          GREATER   );

    check( "1.4.2_3-1",     "1.4.3-2",      LESS      );
    check( "1.4.2_3-1",     "1.4.2_3-2",    LESS      );
    check( "1.4.2_3-1",     "1.4.2_1-1",    GREATER   );

    check( "1.4.2-alpha2",  "1.4.2-beta1",  LESS      );
    check( "1.4.2a-2",      "1.4.2a-3",     LESS      );
    check( "1.4.2a-2",      "1.4.2b-2",     LESS      );
    check( "1.4.2aa-2",     "1.4.2bb-2",    UNDEFINED );
    check( "1.4.2a1-2",     "1.4.2a2-2",    LESS      );
    check( "1.4.2b1-2",     "1.4.2a2-2",    GREATER   );
    check( "1.4.2beta3",    "1.4.2alpha2",  GREATER   );
    check( "1.4.2-some",    "1.4.2-1",      UNDEFINED );
    check( "1.4.2-1",       "1.4.2-some",   UNDEFINED );

    check( "7.0r63-3",      "7.0r68-1",     LESS      );
    check( "27",            "28e",          LESS      );
  }
  else
    check( argv[ 1 ],       argv[ 2 ],      UNDEFINED, false );
}

#endif // Test

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
