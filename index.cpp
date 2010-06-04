#include <arpa/inet.h>
#include <stdio.h>

#include "index.hpp"
#include "exceptions.hpp"

Index::Index( char *filename )
  : File( filename )
{
  /* Verify version 2 index signature */
  assert( htonl( word( 0 ) ) == 0xff744f63 );

  /* Check version number */
  assert( htonl( word( 1 ) ) == 2 );

  size = htonl( word( 2 + 255 ) );

  assert( get_filesize() == (off_t)sizeof( uint32_t ) * (2 + 256 + 7 * size + 10) );
}
