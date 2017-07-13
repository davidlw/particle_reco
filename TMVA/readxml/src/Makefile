PROGS = readxml_fit readxml
CC = g++ -std=c++14 -O2
INC = -I/usr/include -I../../../ROOT_Util -I../../config_util -I../includes
LIBS = `root-config --libs` -lTMVA -L../../../ROOT_Util -lROOT_Util
DEBUG = -g
CFLAGS = -c `root-config --cflags` -Wall $(DEBUG)
LFLAGS = -Wall $(DEBUG)
OBJS = readxml.o readxml_fit.o
SRCS = $(OBJS:o=cc)

all: $(PROGS)

readxml: readxml.o
	$(CC) $(LFLAGS) $(OBJS) -o $@ $(LIBS)

readxml_fit: readxml_fit.o
	$(CC) $(LFLAGS) $(OBJS) -o $@ $(LIBS)

.cc.o:
	$(CC) $(INC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(PROG)

readxml.o: ../includes/readxml.h ../includes/Tools.h ../includes/uti.h
readxml_fit.o: ../includes/readxml.h ../includes/Tools.h ../includes/uti.h