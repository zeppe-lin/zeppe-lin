//! \file      depresolver.cpp
//! \brief     DepResolver Implementation

#include <cassert>
#include <map>

#include "depresolver.h"

using namespace std;

void DepResolver::addDependency( ssize_t first, ssize_t second )
{
  m_dependencies.push_back( make_pair( first, second ) );
}

bool DepResolver::resolve( list< ssize_t >& result )
{
  return topSort( result );
}

bool DepResolver::topSort( list< ssize_t >& result )
{
  // elt -> number of predecessors
  map< ssize_t, ssize_t > numPreds;

  // elt -> number of successors
  map< ssize_t, list< ssize_t > > successors;

  for ( const auto& [ pkg, dep ]: m_dependencies )
  {
    // make sure every elt is a key in numpreds
    if ( numPreds.find( pkg ) == numPreds.end() )
      numPreds[ pkg ] = 0;

    if ( numPreds.find( dep ) == numPreds.end() )
      numPreds[ dep ] = 0;

    // if they're the same, there's no real dependence
    if ( pkg == dep )
      continue;

    // since pkg < dep, dep gains a pred
    numPreds[ dep ] = numPreds[ dep ] + 1;

    // ... and pkg gains a succ
    successors[ pkg ].push_back( dep );
  }

  // suck up everything without a predecessor
  result.clear();
  for ( const auto& [ first, second ]: numPreds )
  {
    if ( second == 0 )
      result.push_back( first );
  }

  // for everything in answer, knock down the pred count on its
  // successors;
  // note that answer grows *in* the loop

  for ( const auto& elem: result )
  {
    assert( numPreds[ elem ] == 0 );

    numPreds.erase( elem );

    if ( successors.find( elem ) != successors.end() )
    {
      for ( const auto& succ: successors[ elem ] )
      {
        numPreds[ succ ] = numPreds[ succ ] - 1;

        if ( numPreds[ succ ] == 0 )
          result.push_back( succ );
      }
      successors.erase( elem );
    }
  }

  return numPreds.size() == 0;
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
