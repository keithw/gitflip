#ifndef ARROWSTORE_HPP
#define ARROWSTORE_HPP

#include <tr1/unordered_map>
#include <string>

using namespace std;
using namespace std::tr1;

typedef unordered_multimap<string, string> arrow_map_t;

class ArrowStore
{
private:
  arrow_map_t arrow_map;

public:
  ArrowStore( void ) {}
  ~ArrowStore() {}

  void add( string src, string dest )
  {
    arrow_map.insert( arrow_map_t::value_type( dest, src ) );
  }

  int get_size( void ) const
  {
    return arrow_map.size();
  }
};

#endif
