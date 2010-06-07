#ifndef ARROWSTORE_HPP
#define ARROWSTORE_HPP

#include <sqlite3.h>

#include "sha1.hpp"

class ArrowStore
{
private:
  sqlite3 *db;
  sqlite3_stmt *begin_query, *new_arrow_query, *commit_query;

  time_t last_commit_time;

  void begin( void );
  void commit( void );

  int inserts;

public:
  ArrowStore( char *filename );
  ~ArrowStore();

  void add( const sha1 src, const sha1 dest );
};

#endif
