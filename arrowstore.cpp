#include <sqlite3.h>
#include <sys/time.h>

#include "arrowstore.hpp"
#include "exceptions.hpp"

static const char begin_query_str[] = "BEGIN";
static const char new_arrow_query_str[] = "INSERT INTO arrows( src, dest ) VALUES ( ?, ? )";
static const char commit_query_str[] = "COMMIT";

ArrowStore::ArrowStore( char *filename )
{
  last_commit_time = 0;
  inserts = 0;

  unixassert( sqlite3_open_v2( filename, &db, SQLITE_OPEN_READWRITE, NULL ) );

  /* Compile queries */
  assert( SQLITE_OK == sqlite3_prepare_v2( db, begin_query_str,
					   -1, &begin_query, NULL ) );
  assert( SQLITE_OK == sqlite3_prepare_v2( db, new_arrow_query_str,
					   -1, &new_arrow_query, NULL ) );
  assert( SQLITE_OK == sqlite3_prepare_v2( db, commit_query_str,
					   -1, &commit_query, NULL ) );

  /* Begin transaction */
  begin();
}

ArrowStore::~ArrowStore()
{
  commit();
  unixassert( sqlite3_close( db ) );
}

void ArrowStore::begin( void )
{
  /* begin new transaction */
  assert( SQLITE_DONE == sqlite3_step( begin_query ) );
  assert( SQLITE_OK == sqlite3_reset( begin_query ) );
}

void ArrowStore::commit( void )
{
  fprintf( stderr, "Committing (inserts = %d)... ", inserts );

  assert( SQLITE_DONE == sqlite3_step( commit_query ) );
  assert( SQLITE_OK == sqlite3_reset( commit_query ) );

  fprintf( stderr, "done.\n" );
}

void ArrowStore::add( const sha1 src, const sha1 dest )
{
  char src_hex[ 41 ], dest_hex[ 41 ];

  /* Bind SHA-1 hashes to prepared query parameters */
  src.ascii( src_hex );
  assert( SQLITE_OK == sqlite3_bind_text( new_arrow_query,
					  1,
					  src_hex,
					  40,
					  SQLITE_STATIC ) );

  dest.ascii( dest_hex );
  assert( SQLITE_OK == sqlite3_bind_text( new_arrow_query,
					  2,
					  dest_hex,
					  40,
					  SQLITE_STATIC ) );

  
  /* Execute query */
  assert( SQLITE_DONE == sqlite3_step( new_arrow_query ) );

  /* Reset query */
  assert( SQLITE_OK == sqlite3_reset( new_arrow_query ) );

  inserts++;

  /* Should we commit? */
  struct timeval current_time;
  unixassert( gettimeofday( &current_time, NULL ) );

  if ( current_time.tv_sec - last_commit_time > 30 ) {
    commit();
    begin();
    last_commit_time = current_time.tv_sec;
  }
}
