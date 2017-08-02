PROGS = readxml_fit readxml
CC = g++ -std=c++14 -O2
INC = -I/usr/include -I../../../ROOT_Util -I../../config_util -I../../../InputChain -I../includes
LIBS = `root-config --libs` -lTMVA -lXMLIO -L../../../ROOT_Util -lROOT_Util
DEBUG = -g
CFLAGS = -c `root-config --cflags` -Wall $(DEBUG)
LFLAGS = -Wall $(DEBUG)
OBJS = readxml.o readxml_fit.o
SRCS = $(OBJS:o=cc)

all: $(PROGS)

readxml: readxml.o
	$(CC) $(LFLAGS) readxml.o -o $@ $(LIBS)

readxml_fit: readxml_fit.o
	$(CC) $(LFLAGS) readxml_fit.o -o $@ $(LIBS) -lRooFit -lRooFitCore -lRooStats

.cc.o:
	$(CC) $(INC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(PROGS)

readxml.o: ../includes/readxml.h ../includes/Tools.h ../includes/uti.h ../../config_util/Configuration.h ../../../ROOT_Util/strToVect.h ../../../InputChain/InputChain.h
readxml_fit.o: ../includes/readxml.h ../includes/Tools.h ../includes/uti.h ../../config_util/Configuration.h ../../../ROOT_Util/strToVect.h