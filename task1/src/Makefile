# Project: mbf9bin
# Makefile created by Dev-C++ 5.11

CPP      = g++
CC       = gcc
WINDRES  = windres.exe
OBJ      = mbf9bin.o time_ms.o mbf16random.o mbf16.o
LINKOBJ  = mbf9bin.o time_ms.o mbf16random.o mbf16.o
LIBS     = -L/usr/lib/x86_64-linux-gnu -lpthread
INCS     =
CXXINCS  = 
BIN      = mbf9bin
CXXFLAGS = $(CXXINCS) -march=native -O3 -std=gnu++11 -g3 -pedantic
CFLAGS   = $(INCS) -march=native -O3 -std=gnu++11 -g3 -pedantic
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

mbf9bin.o: mbf9bin.cpp time_ms.h mbf16random.h mbf16.h
	$(CPP) -c mbf9bin.cpp -o mbf9bin.o $(CXXFLAGS)

time_ms.o: time_ms.cpp time_ms.h
	$(CPP) -c time_ms.cpp -o time_ms.o $(CXXFLAGS)

mbf16random.o: mbf16random.cpp mbf16random.h squares.h
	$(CPP) -c mbf16random.cpp -o mbf16random.o $(CXXFLAGS)

mbf16.o: mbf16.cpp mbf16.h
	$(CPP) -c mbf16.cpp -o mbf16.o $(CXXFLAGS)
