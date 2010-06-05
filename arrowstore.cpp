#include "arrowstore.hpp"
#include "exceptions.hpp"

void ArrowStore::add( const sha1 src, const sha1 dest )
{
  uint32_t src_id, dest_id;

  /* Resolve hashes to 32-bit incrementing IDs */
  id_map_t::const_iterator i = id_map.find( src );
  if ( i == id_map.end() ) {
    src_id = next_id++;
    id_map[ src ] = src_id;
    sha_map[ src_id ] = src;
  } else {
    src_id = i->second;
  }

  id_map_t::const_iterator j = id_map.find( dest );
  if ( j == id_map.end() ) {
    dest_id = next_id++;
    id_map[ dest ] = dest_id;
    sha_map[ dest_id ] = dest;
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
  FILE *alfa = fopen( "GITFLIP_DB.alfa", "w" );
  assert( alfa );

  assert( id_map.write_metadata( alfa ) );
  assert( id_map.write_nopointer_data( alfa ) );

  unixassert( fclose( alfa ) );

  FILE *beta = fopen( "GITFLIP_DB.beta", "w" );
  assert( beta );

  assert( sha_map.write_metadata( beta ) );
  assert( sha_map.write_nopointer_data( beta ) );

  unixassert( fclose( beta ) );

  FILE *arrowfile = fopen( "GITFLIP_DB.data", "w" );
  assert( arrowfile );

  uint32_t total_size = arrows.size();
  assert( 1 == fwrite( &total_size, sizeof( uint32_t ), 1, arrowfile ) );

  for ( size_t i = 0; i < total_size; i++ ) {
    uint32_t vector_len = arrows.at( i ).size();
    assert( 1 == fwrite( &vector_len, sizeof( uint32_t ), 1, arrowfile ) );

    for ( size_t j = 0; j < vector_len; j++ ) {
      uint32_t val = arrows.at( i ).at( j );
      assert( 1 == fwrite( &val, sizeof( uint32_t ), 1, arrowfile ) );
    }
  }

  unixassert( fclose( arrowfile ) );
}

void ArrowStore::readin( void )
{
  FILE *alfa = fopen( "GITFLIP_DB.alfa", "r" );
  assert( alfa );

  fprintf( stderr, "Reading hash-to-id mapping... " );
  assert( id_map.read_metadata( alfa ) );
  assert( id_map.read_nopointer_data( alfa ) );
  fprintf( stderr, "done.\n" );

  unixassert( fclose( alfa ) );

  FILE *beta = fopen( "GITFLIP_DB.beta", "r" );
  assert( beta );

  fprintf( stderr, "Reading id-to-hash mapping... " );
  assert( sha_map.read_metadata( beta ) );
  assert( sha_map.read_nopointer_data( beta ) );
  fprintf( stderr, "done.\n" );

  unixassert( fclose( beta ) );

  FILE *arrowfile = fopen( "GITFLIP_DB.data", "r" );
  assert( arrowfile );

  uint32_t total_size;
  fprintf( stderr, "Reading arrows... " );
  assert( 1 == fread( &total_size, sizeof( uint32_t ), 1, arrowfile ) );

  arrows.resize( total_size );

  int arrow_count = 0;

  for ( size_t i = 0; i < total_size; i++ ) {
    uint32_t vector_len;
    assert( 1 == fread( &vector_len, sizeof( uint32_t ), 1, arrowfile ) );

    arrows.at( i ).resize( vector_len );

    for ( size_t j = 0; j < vector_len; j++ ) {
      uint32_t val;
      assert( 1 == fread( &val, sizeof( uint32_t ), 1, arrowfile ) );
      arrows.at( i ).at( j ) = val;
      arrow_count++;
    }
  }

  fprintf( stderr, "done.\n" );
  unixassert( fclose( arrowfile ) );

  fprintf( stderr, "Number of arrows: %d\n", arrow_count );
}

vector<sha1> ArrowStore::query( const sha1 hash ) const
{
  vector<sha1> ret;

  /* Translate sha1 to id */
  id_map_t::const_iterator findid = id_map.find( hash );
  if ( findid == id_map.end() ) {
    return ret;
  }

  uint32_t id = findid->second;

  ret.resize( arrows.at( id ).size() );

  for ( size_t i = 0; i < arrows.at( id ).size(); i++ ) {
    sha_map_t::const_iterator findsha = sha_map.find( arrows.at( id ).at( i ) );
    if ( findsha == sha_map.end() ) {
      throw InternalError();
    }
    ret.at( i ) = findsha->second;
  }

  return ret;
}
