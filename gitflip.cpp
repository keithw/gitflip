#include <stdio.h>
#include <iostream>

#include "exceptions.hpp"
#include "pack.hpp"
#include "deltadb.hpp"

int main( int argc, char *argv[] )
{
  if ( argc != 3 ) {
    fprintf( stderr, "Usage: %s PACK_FILENAME INDEX_FILENAME\n",
	     argv[ 0 ] );
    return 1;
  }

  char *pack_filename = argv[ 1 ];
  char *idx_filename = argv[ 2 ];

  try {
    Pack *pack = new Pack( pack_filename, idx_filename );
    DeltaDB *deltas = new DeltaDB( pack );

    fprintf( stderr, "Traversed data len: %d.\n", deltas->traverse_all() );

    fprintf( stderr, "Destroying objects... " );
    delete deltas;
    delete pack;
    fprintf( stderr, "done.\n" );
  } catch ( Exception *e ) {
    cout << e->str();
    return 1;
  }

  return 0;
}
