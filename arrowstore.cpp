#include "arrowstore.hpp"

void ArrowStore::add( const sha1 src, const sha1 dest )
{
  uint32_t src_id, dest_id;

  /* Resolve hashes to 32-bit incrementing IDs */
  id_map_t::const_iterator i = id_map.find( src );
  if ( i == id_map.end() ) {
    src_id = next_id++;
    id_map[ src ] = src_id;
  } else {
    src_id = i->second;
  }

  id_map_t::const_iterator j = id_map.find( dest );
  if ( j == id_map.end() ) {
    dest_id = next_id++;
    id_map[ dest ] = dest_id;
  } else {
    dest_id = j->second;
  }

  /* Store new arrow */
  if ( dest_id >= arrows.size() ) {
    arrows.resize( dest_id + 1 );
  }

  arrows.at( dest_id ).push_back( src_id );
}

int ArrowStore::get_size( void ) const
{
  int size = 0;
  for ( size_t i = 0; i < arrows.size(); i++ ) {
    size += arrows.at( i ).size();
  }

  return size;
}
