#include "deltadb.hpp"
#include "objects.hpp"

DeltaDB::DeltaDB( Pack *pack )
{
  /* Track all the objects in the pack file */
  for ( unsigned int i = 0; i < pack->get_object_count(); i++ ) {
    GitObject *obj = pack->make_object( i );

    hash_map.insert( hash_map_t::value_type( obj->get_hash()->str(),
					     obj ) );
    index_map.insert( index_map_t::value_type( obj->get_header_index(),
					       obj ) );
  }

  /* Resolve all delta references */
  for ( hash_map_t::iterator i = hash_map.begin();
        i != hash_map.end();
        i++ ) {
    GitObject *obj = i->second;
    obj->resolve( this );
  }

  /* Print chains */
  for ( hash_map_t::iterator i = hash_map.begin();
        i != hash_map.end();
        i++ ) {
    GitObject *obj = i->second;    
    Delta *delta;

    while ( (delta = dynamic_cast<Delta *>( obj )) ) {
      delta->get_hash()->print();
      fprintf( stderr, " references " );
      delta->get_reference()->get_hash()->print();
      fprintf( stderr, " : " );
      obj = delta->get_reference();
    }
    fprintf( stderr, "\n\n" );
  }
}

DeltaDB::~DeltaDB()
{
  for ( hash_map_t::iterator i = hash_map.begin();
        i != hash_map.end();
        i++ ) {
    GitObject *obj = i->second;
    delete obj;
  }
}

GitObject* DeltaDB::lookup_hash( const sha1 hash ) const
{
  hash_map_t::const_iterator i = hash_map.find( hash.str() );
  assert( i != hash_map.end() );
  assert( i->second );
  return i->second;
}

GitObject* DeltaDB::lookup_header_index( const off_t header_index ) const
{
  index_map_t::const_iterator i = index_map.find( header_index );
  assert( i != index_map.end() );
  assert( i->second );
  return i->second;
}
