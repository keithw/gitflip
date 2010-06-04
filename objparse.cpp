#include <string.h>

#include "objects.hpp"
#include "arrowstore.hpp"
#include "deltadb.hpp"

/* These are member functions of the base object in a delta chain.
   They parse either the base object or a derived, decoded delta,
   based on its type. (A delta doesn't know its own type.)
*/

void Commit::parse( GitObject *obj, ArrowStore *arrows, const DeltaDB *db )
{
  /* confirm "tree" */
  unixassert( strncmp( (char *)obj->get_buf( 0, 5 ), "tree ", 5 ) );
  sha1 tree;
  tree.read( obj->get_buf( 5, 40 ) );
  arrows->add( obj, db->lookup_hash( tree ) );

  /* get as many parents as are present */
  /* we assume every commit has an author and commiter,
     so there's no way our search for "parent " will cause an
     exception for running off the end of the buffer */
  off_t i = 46;
  while ( 0 == strncmp( (char *)obj->get_buf( i, 7 ), "parent ", 7 ) ) {
    sha1 parent;
    parent.read( obj->get_buf( i + 7, 40 ) );
    arrows->add( obj, db->lookup_hash( parent ) );
    i += 48;
  }
}

void Tree::parse( GitObject *obj, ArrowStore *arrows, const DeltaDB *db )
{
  /* null bytes signal end of filename, and immediately precede SHA-1 */
  /* note tree uses binary SHA-1, unlike ASCII form */

  off_t i = 0;
  while ( i < obj->get_delta_decoded_size() ) {
    if ( (*obj)[ i ] == 0 ) {
      if ( i + 21 > obj->get_delta_decoded_size() ) {
	printf( "Zero found at byte %d of ", (int)i );
	obj->get_hash()->print();
	printf( "\n" );

	for ( off_t q = i - 20; q < obj->get_delta_decoded_size(); q++ ) {
	  printf( "%d (%c) ", (*obj)[ q ], (*obj)[ q ] );
	}
	printf( "\n" );
      }
      sha1 entry( (char *)(obj->get_buf( i + 1, 20 ) ) );
      arrows->add( obj, db->lookup_hash( entry ) );
      i += 21;
    } else {
      i++;
    }
  }
}

void Tag::parse( GitObject *obj, ArrowStore *arrows, const DeltaDB *db )
{
  /* confirm beginning */
  unixassert( strncmp( (char *)obj->get_buf( 0, 7 ), "object ", 7 ) );

  sha1 reference;
  reference.read( obj->get_buf( 7, 40 ) );
  arrows->add( obj, db->lookup_hash( reference ) );
}

void Blob::parse( GitObject *obj, ArrowStore *arrows, const DeltaDB *db )
{
  /* We're not interested in these. */
  throw InternalError();
}

void Delta::parse( GitObject *obj, ArrowStore *arrows, const DeltaDB *db )
{
  /* Delta should never be the base of a delta chain. */
  throw InternalError();
}
