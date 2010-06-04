#include <stdio.h>
#include <iostream>

#include "exceptions.hpp"
#include "pack.hpp"
#include "deltadb.hpp"
#include "arrowstore.hpp"

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
    ArrowStore *arrows = new ArrowStore();
    Pack *pack = new Pack( pack_filename, idx_filename );
    DeltaDB *deltas = new DeltaDB( pack, arrows );

    deltas->traverse_all();

    printf( "Number of arrows: %d.\n", arrows->get_size() );

    fprintf( stderr, "Freeing memory... " );
    delete arrows;
    delete deltas;
    delete pack;
    fprintf( stderr, "done.\n" );
  } catch ( Exception *e ) {
    cout << e->str();
    return 1;
  }

  return 0;
}
