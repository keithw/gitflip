#ifndef ARROWSTORE_HPP
#define ARROWSTORE_HPP

#include <tr1/unordered_map>
#include <string>

using namespace std;
using namespace std::tr1;

class GitObject;

typedef unordered_multimap<GitObject *, GitObject *> arrow_map_t;

class ArrowStore
{
private:
  arrow_map_t arrow_map;

public:
  ArrowStore( void ) {}
  ~ArrowStore() {}

  void add( GitObject *src, GitObject *dest )
  {
    arrow_map.insert( arrow_map_t::value_type( dest, src ) );
  }

  int get_size( void ) const
  {
    return arrow_map.size();
  }
};

#endif
