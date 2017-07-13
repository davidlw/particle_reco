PROG = TMVAClassification
CC = g++ -std=c++14
INC = -I/usr/include -I../../../ROOT_Util -I../../config_util
LIBS = `root-config --libs` -lTMVA -L../../ROOT_Util -lROOT_Util
DEBUG = -g
CFLAGS = -c `root-config --cflags` -Wall $(DEBUG)
LFLAGS = -Wall $(DEBUG)
OBJS = TMVAClassification.o
SRCS = $(OBJS:o=C)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $@ $(LIBS)

TMVAClassification.o: TMVAClassification.C
	$(CC) $(INC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(PROG)