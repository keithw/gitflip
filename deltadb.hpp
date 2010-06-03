#ifndef DELTADB_HPP
#define DELTADB_HPP

#include <tr1/unordered_map>
#include <string>

#include "objects.hpp"
#include "pack.hpp"

using namespace std;

typedef std::tr1::unordered_map<string, GitObject *> hash_map_t;
typedef std::tr1::unordered_map<off_t, GitObject *> index_map_t;

class DeltaDB
{
private:
  hash_map_t hash_map;
  index_map_t index_map;

public:
  DeltaDB( Pack *pack );
  ~DeltaDB();

  GitObject* lookup_hash( const sha1 hash ) const;
  GitObject* lookup_header_index( const off_t header_index ) const;
};

#endif
