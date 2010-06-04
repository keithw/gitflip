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
  
  post_inflate_hook();
}

void Delta::apply_delta( GitObject *parent )
{
  assert( parent );

  /* First syntax element in delta format: parent size */
  off_t i = 0;
  size_t parent_size = 0;
  int shift = 0;
  while ( i < size ) {
    uint8_t byte = decoded_data[ i++ ];
    parent_size |= (byte & 0x7f) << shift;
    shift += 7;
    if ( !(byte & 0x80) ) break;
  }

  assert( parent_size == parent->get_delta_decoded_size() );

  /* Next syntax element: our target size */
  size_t target_size = 0;
  shift = 0;
  while ( i < size ) {
    uint8_t byte = decoded_data[ i++ ];
    target_size |= (byte & 0x7f) << shift;
    shift += 7;
    if ( !(byte & 0x80) ) break;
  }

  delta_decoded_size = target_size;
  delta_decoded_data = new uint8_t[ delta_decoded_size ];

  /* Now apply deltas. Taken from git patch-delta.c */
  uint8_t *out = delta_decoded_data;

  const uint8_t *data = decoded_data + i;
  const uint8_t *top = decoded_data + size;

  size_t size_tmp = target_size;

  while (data < top) {
    uint8_t cmd = *data++;
    if (cmd & 0x80) {
      unsigned long cp_off = 0, cp_size = 0;
      if (cmd & 0x01) cp_off = *data++;
      if (cmd & 0x02) cp_off |= (*data++ << 8);
      if (cmd & 0x04) cp_off |= (*data++ << 16);
      if (cmd & 0x08) cp_off |= ((unsigned) *data++ << 24);
      if (cmd & 0x10) cp_size = *data++;
      if (cmd & 0x20) cp_size |= (*data++ << 8);
      if (cmd & 0x40) cp_size |= (*data++ << 16);
      if (cp_size == 0) cp_size = 0x10000;
      if (cp_off + cp_size < cp_size ||
	  cp_off + cp_size > parent_size ||
	  cp_size > size_tmp)
	break;
      memcpy(out, (char *) parent->delta_decoded_data + cp_off, cp_size);
      out += cp_size;
      size_tmp -= cp_size;
    } else if (cmd) {
      if (cmd > size_tmp)
	break;
      memcpy(out, data, cmd);
      out += cmd;
      data += cmd;
      size_tmp -= cmd;
    } else {
      throw InternalError();
    }
  }
  
  if (data != top || size_tmp != 0) {
    throw InternalError();
  }
}
