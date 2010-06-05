#ifndef SHA1_HPP
#define SHA1_HPP

#include <stdio.h>
#include <string>

using namespace std;

class sha1
{
public:
  unsigned char hash[ 20 ];
  sha1( const sha1 &source ) { memcpy( hash, source.hash, 20 ); }
  sha1( const char *s_hash ) { memcpy( hash, s_hash, 20 ); }
  sha1() {}
  void print( void ) const
  {
    for ( int i = 0; i < 20; i++ ) {
      printf( "%02x", hash[ i ] );
    }
  }
  
  void read( const uint8_t *ascii )
  {
    for ( int i = 0; i < 20; i++ ) {
      unsigned int byte;
      assert( 1 == sscanf( (char *)ascii + 2 * i, "%02x", &byte ) );
      hash[ i ] = byte;
    }
  }
  const string str( void ) const
  {
    char *cstr = (char*) &(hash[0]);
    const string x( cstr, 20 );
    return x;
  }
};

#endif
