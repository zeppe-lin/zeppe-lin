//! \file   versioncomparator.h
//! \brief  VersionComparator namespace definition

#pragma once

#include <string>
#include <vector>

using namespace std;

//! \namespace  VersionComparator
//! \brief      Compare versions of packages
namespace VersionComparator
{
  //! Comparation result
  enum COMP_RESULT
  {
    LESS,       //!< v1 less than v2
    GREATER,    //!< v1 greater than v2
    EQUAL,      //!< v1 equal v2
    UNDEFINED   //!< Undefined comparation
  };

  //! \brief   Compare versions
  //!
  //! \param   v1  version string
  //! \param   v2  version string
  //!
  //! \return  the comparation results of v1 and v2
  COMP_RESULT compareVersions( const string& v1, const string& v2 );

  //! \brief   Subdivide version in blocks
  //!
  //! Subdivide version in blocks, where a block is separated by
  //! any of the following: [-_]
  //!
  //! \code
  //! e.g.
  //!   1.4.2-pre1-2  ->  [ (1.4.2)     (pre1)  (2)         ]
  //!   1.4.2pre1-1   ->  [ (1.4.2pre1) (1)                 ]
  //!   1_2_2pre2-2   ->  [ (1)         (2)     (2pre2) (2) ]
  //! \endcode
  //!
  //! \param   version  the string version
  //! \param   blocks   split into this blocks
  void
    tokenizeIntoBlocks( const string&      version,
                        vector< string >&  blocks );

  //! \brief   Normalize version blocks size
  //!
  //! Since, after \a tokenizeIntoBlocks() the resulting blocks might
  //! have the different length, we add to that one that is smaller
  //! "-1" for iterating.
  //!
  //! \param   v1  first version blocks
  //! \param   v2  second version blocks
  //!
  //! \return  the size of max block
  size_t
    normalizeBlocksSize( vector< string >&  v1,
                         vector< string >&  v2 );

  //! \brief   Subdivide version tokens into subtokens by mask
  //!
  //! Subdivide version tokens into subtokens, where a block is
  //! separated by mask (isdigit).
  //!
  //! \code
  //! e.g.
  //!   1.4.2   ->  [ (1)   (.) (4) (.) (2) ]
  //!   pre1    ->  [ (pre) (1)             ]
  //!   14.2    ->  [ (14)  (.) (2)         ]
  //! \endcode
  //!
  //! \param   s       the version token
  //! \param   tokens  split into this block
  void
    tokenizeMixedIntoBlocks( const string&      s,
                             vector< string >&  tokens );
}

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
