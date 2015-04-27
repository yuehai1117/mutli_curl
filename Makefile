OUTPUTFILES=main

CXXFLAGS=-I./
LIBS=-L. -lpthread -lcurl


CXX=g++ -g -Wall -fPIC -fno-strict-aliasing 
CC=gcc -g -Wall -fPIC -fno-strict-aliasing 
#---------------------------------------------------------------------------------------------

all: $(OUTPUTFILES)

.SUFFIXES: .o .cpp .hpp
#---------------------------------------------------------------------------------------------

main: main.o mutli_curl.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) 

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<
.c.o:
	$(CC) $(CXXFLAGS) -c $<
#---------------------------------------------------------------------------------------------

clean:
	rm -rf $(OUTPUTFILES) *.o *.so *.a *~
