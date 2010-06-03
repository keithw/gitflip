#include <stdio.h>

#include "exceptions.hpp"

void failure( const char *assertion, const char *file, int line, const char *function, Exception *e )
{
  fprintf( stderr, "\nAssertion \"%s\" failed in file %s, function %s(), line #%d\n",
           assertion, file, function, line );

  throw e;
}

string UnixAssertError::str( void )
{
  char msg[ 2048 ];
  msg[ 2047 ] = 0;
  snprintf( msg, 2047, "Statement \"%s\" failed in file %s, function %s(), line %d.\nReturn value was %d, errno = %d (%s).\n",
	    expr, file, function, line, result, errnumber, strerror( errnumber ) );
  return string( msg );
}
