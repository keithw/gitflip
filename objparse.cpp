#include <string.h>

#include "objects.hpp"
#include "arrowstore.hpp"

/* These are member functions of the base object in a delta chain.
   They parse either the base object or a derived, decoded delta,
   based on its type. (A delta doesn't know its own type.)
*/

void Commit::parse( GitObject *obj, ArrowStore *arrows )
{
  /* confirm "tree" */
  unixassert( strncmp( (char *)obj->get_buf( 0, 5 ), "tree ", 5 ) );
  sha1 tree;
  tree.read( obj->get_buf( 5, 40 ) );
  arrows->add( obj->get_hash()->str(), tree.str() );

  /* get as many parents as are present */
  /* we assume every commit has an author and commiter,
     so there's no way our search for "parent " will cause an
     exception for running off the end of the buffer */
  int i = 46;
  while ( 0 == strncmp( (char *)obj->get_buf( i, 7 ), "parent ", 7 ) ) {
    sha1 parent;
    parent.read( obj->get_buf( i + 7, 40 ) );
    arrows->add( obj->get_hash()->str(), parent.str() );
    i += 48;
  }
}

void Tree::parse( GitObject *obj, ArrowStore *arrows )
{
}

void Tag::parse( GitObject *obj, ArrowStore *arrows )
{
  /* confirm beginning */
  unixassert( strncmp( (char *)obj->get_buf( 0, 7 ), "object ", 7 ) );

  sha1 reference;
  reference.read( obj->get_buf( 7, 40 ) );
  arrows->add( obj->get_hash()->str(), reference.str() );
}

void Blob::parse( GitObject *obj, ArrowStore *arrows )
{
  /* We're not interested in these. */
  throw InternalError();
}

void Delta::parse( GitObject *obj, ArrowStore *arrows )
{
  /* Delta should never be the base of a delta chain. */
  throw InternalError();
}
