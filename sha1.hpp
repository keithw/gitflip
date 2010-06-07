#ifndef SHA1_HPP
#define SHA1_HPP

#include <stdio.h>
#include <string>
#include <string.h>

#include "exceptions.hpp"

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

  void ascii( char *target ) const
  {
    sprintf( target,
	     "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	     hash[0], hash[1], hash[2], hash[3], hash[4],
	     hash[5], hash[6], hash[7], hash[8], hash[9],
	     hash[10], hash[11], hash[12], hash[13], hash[14],
	     hash[15], hash[16], hash[17], hash[18], hash[19] );	     
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
