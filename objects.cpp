#include <stdio.h>
#include <stdint.h>

#include "objects.hpp"
#include "pack.hpp"
#include "deltadb.hpp"

enum TypeCode { OBJ_COMMIT = 1,
		OBJ_TREE,
		OBJ_BLOB,
		OBJ_TAG,

		OBJ_OFS_DELTA = 6,
		OBJ_REF_DELTA };

GitObject* GitObject::make_object( const Pack *pack, off_t header_index, const sha1 hash )
{
  off_t index = header_index;

  uint8_t byte = pack->byte( index++ );
  uint8_t type = (byte >> 4) & 7;
  size_t size = byte & 0x0f;
  uint8_t shift = 4;

  while ( byte & 0x80 ) {
    byte = pack->byte( index++ );
    size += (byte & 0x7f) << shift;
    shift += 7;
  }
  
  GitObject *obj = NULL;

  switch ( type ) {
  case OBJ_COMMIT:
    obj = new Commit();
    break;
  case OBJ_TREE:
    obj = new Tree();
    break;
  case OBJ_BLOB:
    obj = new Blob();
    break;
  case OBJ_TAG:
    obj = new Tag();
    break;
  case OBJ_OFS_DELTA:
    obj = new Ofs_Delta();
    break;
  case OBJ_REF_DELTA:
    obj = new Ref_Delta();
    break;
  default:
    throw InternalError();
  }

  obj->init( pack, index, header_index, hash, size );
  
  return obj;
}

void Ofs_Delta::decode_delta_ptr( void )
{
  off_t i = data_index;
  uint8_t byte = pack->byte( i++ );
  size_t negative_offset = byte & 0x7f;
  while ( byte & 0x80 ) {
    byte = pack->byte( i++ );
    negative_offset = ((negative_offset+1) << 7) + (byte & 0x7f);
  }

  reference_header_index = header_index - negative_offset;
}

void Ref_Delta::decode_delta_ptr( void )
{
  /* Access will not necessarily be word-aligned here */
  for ( int i = 0; i < 20; i++ ) {
    reference.hash[ i ] = pack->byte( data_index + i );
  }
}

bool Ofs_Delta::resolve( DeltaDB *db )
{
  reference_object = db->lookup_header_index( reference_header_index );
  return true;
}

bool Ref_Delta::resolve( DeltaDB *db )
{
  reference_object = db->lookup_hash( reference );
  return true;
}
