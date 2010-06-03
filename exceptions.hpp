#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <string>

using namespace std;

class Exception {
public:
  virtual ~Exception() {}
  virtual string str( void ) = 0;
};

class InternalError : public Exception { string str( void ) { return string( "Internal Error" ); } };
class UnixError : public Exception
{
public:
  int err;
  UnixError( int s_errno ) { err = s_errno; }
  string str( void ) { return string( strerror( err ) ); }
};
class UnixAssertError : public Exception
{
private:
  char *expr, *file, *function;
  int line, result, errnumber;
  
public:
  UnixAssertError( const char *s_expr, const char *s_file, int s_line, const char *s_function,
                   int s_result, int s_errnumber )
  {
    expr = strdup( s_expr );
    file = strdup( s_file );
    line = s_line;
    function = strdup( s_function );
    result = s_result;
    errnumber = s_errnumber;
  }

  string str( void );
};

void failure( const char *assertion, const char *file, int line, const char *function, Exception *e );

void warn( const char *assertion, const char *file, int line, const char *function );

#define unixassert(expr) do {						\
	     int assertion_result = (expr);				\
	     if ( assertion_result != 0 ) {				\
	       throw new UnixAssertError( __STRING(expr), __FILE__, __LINE__, __PRETTY_FUNCTION__, assertion_result, errno ); \
	     } } while ( 0 )

#define assert(expr)							\
	   ((expr)							\
	    ? (void)0							\
	    : failure (__STRING(expr), __FILE__, __LINE__, __PRETTY_FUNCTION__, new InternalError() ))

#endif
