#include <stdio.h>
#include <iostream>

#include "exceptions.hpp"
#include "pack.hpp"

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

  try {
    pack = new Pack( pack_filename, idx_filename );
  } catch ( Exception *e ) {
    cout << e->str();
    return 1;
  }

  for ( unsigned int i = 0; i < pack->get_object_count(); i++ ) {
    GitObject *obj = pack->make_object( i );
    delete obj;
  }

  delete pack;

  return 0;
}
