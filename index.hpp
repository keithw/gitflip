#ifndef INDEX_HPP
#define INDEX_HPP

#include <arpa/inet.h>

#include "file.hpp"
#include "exceptions.hpp"
#include "objects.hpp"

class Index : public File
{
private:
  uint32_t size;

public:
  Index( char *filename );

  uint32_t get_size( void ) const { return size; }

  uint32_t offset_of( const uint32_t object_num ) const
  {
    assert( object_num >= 0 );
    assert( object_num < size );

    uint32_t offset = htonl( word( 2 + 256 + 6 * size + object_num ) );
    assert( !(offset & (0x1 << 31)) );

    return offset;
  }

  sha1 hash_of( const uint32_t object_num ) const
  {
    char hash[ 20 ];
    assert( object_num >= 0 );
    assert( object_num < size );

    off_t start = 2 + 256 + 5*object_num;

    for ( off_t i = 0; i < 5; i++ ) {
      uint32_t myword = word( start + i );
      memcpy( hash + i*4, &myword, 4 );
    }

    return sha1( hash );
  }
};

#endif
