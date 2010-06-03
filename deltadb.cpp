#include <typeinfo>
#include <iostream>

#include "deltadb.hpp"
#include "objects.hpp"

DeltaDB::DeltaDB( Pack *pack )
{
  /* Track all the objects in the pack file */
  fprintf( stderr, "Ingesting objects... " );
  for ( unsigned int i = 0; i < pack->get_object_count(); i++ ) {
    GitObject *obj = pack->make_object( i );

    hash_map.insert( hash_map_t::value_type( obj->get_hash()->str(),
					     obj ) );
    index_map.insert( index_map_t::value_type( obj->get_header_index(),
					       obj ) );
  }
  fprintf( stderr, "done.\n" );

  /* Resolve all delta references */
  fprintf( stderr, "Resolving delta references... " );
  for ( hash_map_t::iterator i = hash_map.begin(); i != hash_map.end(); i++ ) {
    GitObject *obj = i->second;
    if ( obj->resolve( this ) ) {
      /* Flip the delta-dependency graph */
      Delta *delta = static_cast<Delta *>( obj );
      assert( delta );

      GitObject *parent = delta->get_reference();
      assert( parent );

      child_map.insert( child_map_t::value_type( parent, delta ) );
    } else {
      /* Object is not a delta type */
      base_map.insert( base_map_t::value_type( obj, true ) );
    }
  }
  fprintf( stderr, "done.\n" );
}

unsigned int DeltaDB::traverse_all( void ) const {
  /* Follow all base objects (non-deltas) to deltas */
  fprintf( stderr, "Traversing all delta chains... " );

  unsigned int size = 0;
  for ( base_map_t::const_iterator i = base_map.begin(); i != base_map.end(); i++ ) {
    GitObject *obj = i->first;
    if ( (typeid( *obj ) == typeid( Commit ))
	 || (typeid( *obj ) == typeid( Tree ))
	 || (typeid( *obj ) == typeid( Tag )) ) {
      size += recursive_traverse( obj, NULL );
    } else if ( typeid( *obj ) == typeid( Blob ) ) {
      /* do nothing */
    } else {
      throw InternalError();
    }
  }

  fprintf( stderr, "done.\n" );

  return size;
}

int DeltaDB::recursive_traverse( GitObject *obj, GitObject *parent ) const
{
  std::pair< child_map_t::const_iterator, child_map_t::const_iterator >
    children = child_map.equal_range( obj );

  int size = obj->get_size();
  obj->inflate();
  obj->apply_delta( parent );

  for ( child_map_t::const_iterator i = children.first; i != children.second; i++ ) {
    GitObject *child = i->second;
    size += recursive_traverse( child, obj );
  }

  return size;
}

DeltaDB::~DeltaDB()
{
  for ( hash_map_t::iterator i = hash_map.begin(); i != hash_map.end(); i++ ) {
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
