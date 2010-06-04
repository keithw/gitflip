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

void ArrowStore::writeout( void )
{
  FILE *hash = fopen( "GITFLIP_DB.index", "w" );
  assert( hash );

  id_map.write_metadata( hash );
  id_map.write_nopointer_data( hash );

  unixassert( fclose( hash ) );

  FILE *arrowfile = fopen( "GITFLIP_DB.data", "w" );
  assert( arrowfile );

  for ( size_t i = 0; i < arrows.size(); i++ ) {
    uint32_t vector_len = arrows.at( i ).size();
    fwrite( &vector_len, sizeof( uint32_t ), 1, arrowfile );

    for ( size_t j = 0; j < vector_len; j++ ) {
      uint32_t val = arrows.at( i ).at( j );
      fwrite( &val, sizeof( uint32_t ), 1, arrowfile );
    }
  }

  unixassert( fclose( arrowfile ) );
}
