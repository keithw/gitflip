#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "file.hpp"

File::File( char *filename )
{
  /* Open file */
  fd = open( filename, O_RDONLY );
  if ( fd < 0 ) {
    perror( "open" );
    throw UnixError( errno );
  }

  /* Get size of file */
  struct stat thestat;

  unixassert( fstat( fd, &thestat ) );

  filesize = thestat.st_size;

  /* Map file */
  buffer = (uint8_t *)mmap( NULL, filesize, PROT_READ, MAP_SHARED, fd, 0 );

  if ( buffer == MAP_FAILED ) {
    perror( "mmap" );
    throw UnixError( errno );
  }  
}

File::~File()
{
  unixassert( munmap( buffer, filesize ) );
  unixassert( close( fd ) );
}
