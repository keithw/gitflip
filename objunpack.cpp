#ifndef OBJUNPACK_HPP
#define OBJUNPACK_HPP

#include <zlib.h>

#include "objects.hpp"
#include "pack.hpp"

void GitObject::inflate_object( void )
{
  uint8_t *compressed = pack->get_buf( data_index, size );

  assert( decoded_data );

  z_stream zstr;
  zstr.next_in = compressed;
  zstr.avail_in = pack->get_filesize() - data_index;
  zstr.zalloc = Z_NULL;
  zstr.zfree = Z_NULL;
  zstr.opaque = NULL;
  zstr.next_out = decoded_data;
  zstr.avail_out = size;

  assert( inflateInit( &zstr ) == Z_OK );
  
  assert( inflate( &zstr, Z_FINISH ) == Z_STREAM_END );

  assert( inflateEnd( &zstr ) == Z_OK );
}

void Delta::apply_delta( GitObject *parent )
{

}

#endif
