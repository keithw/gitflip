#ifndef ARROWSTORE_HPP
#define ARROWSTORE_HPP

#include <google/sparse_hash_map>
#include <google/sparse_hash_set>
#include <string>

#include "sha1.hpp"

using namespace std;

using google::sparse_hash_map;

struct eqsha
{
  bool operator() ( const sha1 s1, const sha1 s2 ) const
  {
    return (0 == memcmp( s1.hash, s2.hash, 20 ));
  }
};

struct eqint
{
  bool operator() ( const uint32_t x, const uint32_t y ) const
  {
    return (x == y);
  }
};

/* The first 32 bits of a SHA-1 are a fine hash for the whole thing */
struct sha_hash
{
  size_t operator() ( const sha1 x ) const
  {
    return (x.hash[ 0 ] << 24) | (x.hash[ 1 ] << 16) | (x.hash[ 2 ] << 8 ) | x.hash[ 3 ];
  }
};

struct int_hash
{
  size_t operator() ( const uint32_t x ) const
  {
    return (size_t) x;
  }
};

typedef sparse_hash_map<sha1, uint32_t, sha_hash, eqsha> id_map_t;
typedef sparse_hash_map<uint32_t, sha1, int_hash, eqint> sha_map_t;

class ArrowStore
{
private:
  id_map_t id_map;
  sha_map_t sha_map;

  int next_id;

  vector< vector<uint32_t> > arrows;

public:
  ArrowStore( void ) { next_id = 0; }
  ~ArrowStore() {}

  void add( const sha1 src, const sha1 dest );

  int get_size( void ) const;

  void writeout( void );
};

#endif
