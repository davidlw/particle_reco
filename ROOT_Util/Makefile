LIB = ROOT_Util
CC = g++ -std=c++14 `root-config --libs --cflags`
DEBUG = -g
CFLAGS = -c -Wall $(DEBUG)
OBJS = calcPartition.o doubleToStr.o drawMany.o loadChain.o
SRCS = $(OBJS:o=cpp)

all: lib$(LIB).a

lib$(LIB).a: $(OBJS)
	ar rc $@ $(OBJS)
	ranlib $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o lib$(LIB).a

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)


# DO NOT DELETE

calcPartition.o: calcPartition.h
doubleToStr.o: doubleToStr.h
drawMany.o: drawMany.h calcPartition.h
loadChain.o: loadChain.h
