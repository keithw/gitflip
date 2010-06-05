#include <iostream>

#include "arrowstore.hpp"
#include "exceptions.hpp"

/* Query program. Takes ASCII SHA-1's (followed by newline) on stdin,
   and prints out the objects that depend on the given object. */

int main( void )
{
  ArrowStore *arrows = new ArrowStore();

  try {
    arrows->readin();
  } catch ( Exception *e ) {
    cout << e->str();
    return 1;
  }

  while ( 1 ) {
    char ascii_sha[ 41 ];
    int count = fread( ascii_sha, 41, 1, stdin );
    if ( (count != 1) || (ascii_sha[40] != '\n') ) {
      break;
    }
    
    sha1 hash;
    hash.read( (uint8_t *)ascii_sha );

    vector<sha1> objects = arrows->query( hash );

    hash.print();
    printf( " <= " );

    for ( size_t i = 0; i < objects.size(); i++ ) {
      objects.at( i ).print();
      printf( " " );
    }
    printf( "\n" );
  } 

  return 0;
}
