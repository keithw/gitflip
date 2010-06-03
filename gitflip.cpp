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

  Pack *pack = NULL;
  DeltaDB *deltas = NULL;

  try {
    pack = new Pack( pack_filename, idx_filename );
    deltas = new DeltaDB( pack );
  } catch ( Exception *e ) {
    cout << e->str();
    return 1;
  }

  delete deltas;
  delete pack;

  return 0;
}
