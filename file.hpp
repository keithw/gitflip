#ifndef FILE_HPP
#define FILE_HPP

#include <stdint.h>
#include <sys/types.h>

#include "exceptions.hpp"

class File
{
private:
  int fd;
  off_t filesize;
  uint8_t *buffer;

public:
  File( char *filename );
  ~File();

  uint8_t byte ( const off_t index ) const
  {
    assert( index >= 0 );
    assert( index < filesize );
    return buffer[ index ];
  }

  uint32_t word ( const off_t index ) const
  {
    assert( index >= 0 );
    assert( (index + 1) * sizeof( uint32_t ) <= filesize );
    return ((uint32_t*)buffer)[ index ];
  }

  off_t get_filesize( void ) const { return filesize; }

  uint8_t operator[] ( const off_t index ) const { return byte( index ); }
};

#endif
