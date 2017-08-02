PROGS = TryCuts TryCuts_fit
CC = g++ -std=c++14 -O2
INC = -I/usr/include -I../../ROOT_Util -I../../InputChain
LIBS = `root-config --libs` -L../../ROOT_Util -lROOT_Util
DEBUG = -g
CFLAGS = -c `root-config --cflags` -Wall $(DEBUG)
LFLAGS = -Wall $(DEBUG)
OBJS = TryCuts.o TryCuts_fit.o Configuration.o
SRCS = $(OBJS:o=cpp)

all: $(PROGS)

TryCuts: TryCuts.o Configuration.o
	$(CC) $(LFLAGS) TryCuts.o Configuration.o -o $@ $(LIBS)

TryCuts_fit: TryCuts_fit.o Configuration.o
	$(CC) $(LFLAGS) TryCuts_fit.o Configuration.o -o $@ $(LIBS) -lRooFitCore -lRooFit

.cpp.o:
	$(CC) $(INC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(PROGS)

TryCuts.o: TryCuts.h Configuration.h ../../InputChain/InputChain.h
TryCuts_fit.o: TryCuts_fit.h Configuration.h
Configuration.o: Configuration.h
