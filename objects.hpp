#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <sys/types.h>
#include <string.h>

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
      fprintf( stderr, "%02x", hash[ i ] );
    }
  }
};

class GitObject
{
private:
  virtual void decode_delta_ptr( void ) = 0;

protected:
  const Pack *pack;
  sha1 hash;
  off_t data_index;
  off_t header_index;
  size_t size;

public:
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
};

class Commit : public GitObject
{
  void decode_delta_ptr( void ) {}
};

class Tree : public GitObject
{
  void decode_delta_ptr( void ) {}
};

class Blob : public GitObject
{
  void decode_delta_ptr( void ) {}
};

class Tag : public GitObject
{
  void decode_delta_ptr( void ) {}
};

class Delta : public GitObject
{
};

class Ofs_Delta : public Delta
{
  off_t reference_header_index;
  void decode_delta_ptr( void );
};

class Ref_Delta : public Delta
{
  sha1 reference;
  void decode_delta_ptr( void );
};

#endif
