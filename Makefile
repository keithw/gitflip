source = gitflip.cpp query.cpp file.cpp pack.cpp index.cpp exceptions.cpp objects.cpp deltadb.cpp objunpack.cpp objparse.cpp arrowstore.cpp
objects = file.o pack.o index.o exceptions.o objects.o deltadb.o objunpack.o objparse.o arrowstore.o
executables = gitflip

CPP = g++
CPPFLAGS = -g -O2 --std=c++0x -pedantic -Werror -Wall -fno-implicit-templates -pipe -D_FILE_OFFSET_BITS=64 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE
LIBS = -lz -lsqlite3

all: $(executables)

gitflip: gitflip.o $(objects)
	$(CPP) $(CPPFLAGS) -o $@ $+ $(LIBS)

deltadb.o: deltadb.cpp
	$(CPP) $(CPPFLAGS) -frepo -c -o $@ $<

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

include depend

depend: $(source)
	$(CPP) $(INCLUDES) -MM $(source) > depend

.PHONY: clean
clean:
	-rm -f $(executables) depend *.o *.rpo
