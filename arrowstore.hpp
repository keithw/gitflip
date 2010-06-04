#ifndef ARROWSTORE_HPP
#define ARROWSTORE_HPP

#include <google/dense_hash_map>
#include <google/dense_hash_set>

using google::dense_hash_map;
using google::dense_hash_set;
using std::tr1::hash;

class GitObject;

struct eqptr
{
  bool operator() ( const GitObject *x, const GitObject *y ) const
  {
    return (x == y);
  }
};

typedef dense_hash_set<const GitObject *, std::tr1::hash<const GitObject *>, eqptr> object_set_t;

typedef dense_hash_map<const GitObject *, object_set_t, std::tr1::hash<const GitObject *>, eqptr> arrow_map_t;

class ArrowStore
{
private:
  arrow_map_t arrow_map;

public:
  ArrowStore( void ) { arrow_map.set_empty_key( NULL ); }
  ~ArrowStore() {}

  void add( GitObject *src, GitObject *dest )
  {
    arrow_map_t::iterator i = arrow_map.find( dest );
    if ( i == arrow_map.end() ) {
      object_set_t set;
      set.set_empty_key( NULL );
      set.insert( src );
      arrow_map.insert( arrow_map_t::value_type( dest, set ) );
    } else {
      i->second.insert( src );
    }
  }

  int get_size( void ) const
  {
    int size = 0;
    for ( arrow_map_t::const_iterator i = arrow_map.begin(); i != arrow_map.end(); i++ ) {
      size += i->second.size();
    }
    return size;
  }
};

#endif
