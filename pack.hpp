#ifndef PACK_HPP
#define PACK_HPP

#include "file.hpp"
#include "index.hpp"
#include "objects.hpp"

class Pack : public File
{
private:
  Index index;
  uint32_t object_count;

public:
  Pack( char *pack_filename, char *idx_filename );
  ~Pack() {}

  uint32_t get_object_count( void ) const { return object_count; }

  GitObject* make_object ( const int i ) const
  {
    return GitObject::make_object( this, index.offset_of( i ),
				   index.hash_of( i ) );
  }
};

#endif
