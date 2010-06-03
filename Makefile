source = gitflip.cpp file.cpp pack.cpp index.cpp exceptions.cpp objects.cpp
objects = file.o pack.o index.o exceptions.o objects.o
executables = gitflip

CPP = g++
CPPFLAGS = -g -O2 --std=c++0x -pedantic -Werror -Wall -fno-implicit-templates -pipe -pthread -D_FILE_OFFSET_BITS=64 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE
LIBS =

all: $(executables)

gitflip: gitflip.o $(objects)
	$(CPP) $(CPPFLAGS) -o $@ $+ $(LIBS)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

include depend

depend: $(source)
	$(CPP) $(INCLUDES) -MM $(source) > depend

.PHONY: clean
clean:
	-rm -f $(executables) depend *.o *.rpo
