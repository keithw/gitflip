#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <sys/types.h>
#include <string.h>
#include <string>

#include "exceptions.hpp"

class DeltaDB;

using namespace std;

class Pack;

class sha1
{
public:
  unsigned char hash[ 20 ];
  sha1( const sha1 &source ) { memcpy( hash, source.hash, 20 ); }
  sha1( const char *s_hash ) { memcpy( hash, s_hash, 20 ); }
  sha1() {}
  void print( void ) const {
    for ( int i = 0; i < 20; i++ ) {
      printf( "%02x", hash[ i ] );
    }
  }
  string str( void ) const
  {
    char *cstr = (char*) &(hash[0]);
    string x( cstr, 20 );
    return x;
  }
};

class GitObject
{
private:
  virtual void decode_delta_ptr( void ) {}

protected:
  const Pack *pack;
  sha1 hash;
  off_t data_index;
  off_t header_index;
  size_t size;

public:
  uint8_t *decoded_data;
  uint8_t *delta_decoded_data;

  GitObject( void )
    : pack( NULL ), decoded_data( NULL ), delta_decoded_data( NULL )
  {
  }

  ~GitObject( void )
  {
    assert( !decoded_data );
  }

  void alloc( void ) { decoded_data = new uint8_t[ size ]; }
  void free( void ) { delete[] decoded_data; decoded_data = NULL; }

  virtual void contents( void ) = 0;
  virtual bool resolve( DeltaDB *db ) { return false; }

  void inflate_object( void );
  virtual void apply_delta( GitObject *parent ) {}

  static GitObject* make_object( const Pack *pack, off_t header_index, const sha1 hash );

  void init( const Pack *s_pack, off_t s_data_index, off_t s_header_index,
	     const sha1 s_hash, uint64_t s_size )
  {
    pack = s_pack;
    data_index = s_data_index;
    header_index = s_header_index;
    hash = s_hash;
    size = s_size;

    this->decode_delta_ptr();
  };

  off_t get_header_index( void ) const { return header_index; }
  const sha1* const get_hash( void ) const { return &hash; }
  size_t get_size( void ) const { return size; }
};

class Commit : public GitObject
{
public:
  void contents( void ) {}
};

class Tree : public GitObject
{
public:
  void contents( void ) {}
};

class Blob : public GitObject
{
public:
  void contents( void ) {}
};

class Tag : public GitObject
{
public:
  void contents( void ) {}
};

class Delta : public GitObject
{
protected:
  GitObject *reference_object;

public:
  Delta() : reference_object( NULL ) {}
  GitObject* get_reference( void ) const { return reference_object; }
  void apply_delta( GitObject *parent );
};

class Ofs_Delta : public Delta
{
private:
  off_t reference_header_index;
  void decode_delta_ptr( void );

public:
  bool resolve( DeltaDB *db );
  void contents( void ) {}
};

class Ref_Delta : public Delta
{
private:
  sha1 reference;
  void decode_delta_ptr( void );

public:
  bool resolve( DeltaDB *db );
  void contents( void ) {}
};

#endif
