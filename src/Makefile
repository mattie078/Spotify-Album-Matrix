CXX = arm-linux-gnueabihf-g++
CC = arm-linux-gnueabihf-gcc

CFLAGS=-Wall -O3 -g -DDISABLE_HARDWARE_PULSES -DDEFAULT_HARDWARE=adafruit-hat
CXXFLAGS=$(CFLAGS)
OBJECTS=spotify-album.o spotify-authentication.o demo-main.o
BINARIES=spotify-album spotify-authentication demo

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

demo : demo-main.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

spotify-album : spotify-album.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

spotify-authentication : spotify-authentication.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS) -lcurl
	
# All the binaries that have the same name as the object file.q
% : %.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

%.o : %.c
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
