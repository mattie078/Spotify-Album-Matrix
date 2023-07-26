CXX = arm-linux-gnueabihf-g++
CC = arm-linux-gnueabihf-gcc

# Output file names
TARGET = librgbmatrix

# List of source files
SOURCE_FILES = gpio.c led-matrix.c options-initialize.c framebuffer.c \
    thread.c bdf-font.c graphics.c led-matrix-c.c hardware-mapping.c \
    pixel-mapper.c multiplex-mappers.c content-streamer.c

# Object files based on source files
OBJECTS = $(SOURCE_FILES:.c=.o)

# Compiler and linker options
INCDIR = ../include
CFLAGS = -W -Wall -Wextra -Wno-unused-parameter -O3 -g -fPIC $(DEFINES)
CXXFLAGS = $(CFLAGS) -fno-exceptions -std=c++11
LIBS_LINUX = -lpthread -lrt -lm -lpthread
LIBS_WINDOWS = # Add any required libraries for Windows here

# Hardware description (comment/uncomment or set via command line)
# HARDWARE_DESC?=regular  # Following this project wiring and using these PCBs
# HARDWARE_DESC=adafruit-hat  # If you have a RGB matrix HAT from Adafruit
# HARDWARE_DESC=adafruit-hat-pwm  # If you have an Adafruit HAT with PWM hardware mod.

# Rule to create the static library
$(TARGET).a: $(OBJECTS)
ifeq ($(PLATFORM),windows)
	lib.exe /OUT:$@ $(OBJECTS)
else
	$(AR) rcs $@ $(OBJECTS)
endif

# Rule to create the shared library (DLL) on Windows
$(TARGET).dll: $(OBJECTS)
	$(CXX) -shared -o $@ $^ $(LIBS_WINDOWS)

# Rule to create the shared library (SO) on Linux
$(TARGET).so.1: $(OBJECTS)
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^ $(LIBS_LINUX)

# Rule to compile C source files to object files
%.o: %.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c -o $@ $<

# Rule to compile C++ source files to object files
%.o: %.cc
	$(CXX) -I$(INCDIR) $(CXXFLAGS) -c -o $@ $<

# Cleanup rule
clean:
	rm -f $(OBJECTS) $(TARGET).a $(TARGET).dll $(TARGET).so.1

.PHONY: clean
