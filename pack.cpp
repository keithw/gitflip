#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "pack.hpp"

Pack::Pack( char *pack_filename, char *idx_filename )
  :    File( pack_filename ),
       index( idx_filename )
{
  /* Verify PACK signature */
  assert( htonl( word( 0 ) ) == 0x5041434b );

  /* Check version number */
  assert( ntohl( word( 1 ) ) == 2 );

  /* Count number of objects */
  object_count = ntohl( word( 2 ) );
  fprintf( stderr, "Object count: %d\n", object_count );

  assert( index.get_size() == object_count );
}
