PROG = TryCuts
CC = g++ -std=c++14
INC = -I/usr/include -I../../ROOT_Util -I../../InputChain
LIBS = `root-config --libs` -L../../ROOT_Util -lROOT_Util
DEBUG = -g
CFLAGS = -c `root-config --cflags` -Wall $(DEBUG)
LFLAGS = -Wall $(DEBUG)
OBJS = TryCuts.o Configuration.o
SRCS = $(OBJS:o=cpp)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(PROG) $(LIBS)

.cpp.o:
	$(CC) $(INC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(PROG)

TryCuts.o: TryCuts.h Configuration.h ../../InputChain.h
Configuration.o: Configuration.h
