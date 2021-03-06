#ifndef DELTADB_HPP
#define DELTADB_HPP

#include <tr1/unordered_map>
#include <string>

#include "objects.hpp"
#include "pack.hpp"

class ArrowStore;

using namespace std;
using namespace std::tr1;

typedef unordered_map<string, GitObject *> hash_map_t;
typedef unordered_map<off_t, GitObject *> index_map_t;
typedef unordered_multimap<GitObject *, GitObject *> child_map_t;
typedef unordered_map<GitObject *, char> base_map_t;

class DeltaDB
{
private:
  hash_map_t hash_map;
  index_map_t index_map;
  child_map_t child_map;
  base_map_t base_map;
  ArrowStore *arrows;

  int recursive_traverse( GitObject *obj, GitObject *parent, GitObject *base ) const;

public:
  DeltaDB( Pack *pack, ArrowStore *s_arrows );
  ~DeltaDB();

  GitObject* lookup_hash( const sha1 hash ) const;
  GitObject* lookup_header_index( const off_t header_index ) const;

  unsigned int traverse_all( void ) const;
};

#endif
