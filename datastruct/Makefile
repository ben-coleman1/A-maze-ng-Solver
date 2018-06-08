# Makefile for an amazing project (you'll see)
# 
# CS50 W18 Group 5


PROG = mazeGraph
OBJS = mazeGraph.o graphics.o

LIB = datastruct.a

GOPT = `pkg-config --cflags gtk+-2.0 --libs`
G = graphics
GOBJS = $(g.o)
g.o = graphics.o


all: $(LIB)

test: $(PROG)

# uncomment the following to turn on verbose memory logging
#TESTING=-DTEST

CFLAGS = -c -Wall -pedantic -std=c11 -ggdb $(TESTING) $(GOPT)
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

gdk: #$(G)
	$(CC) $(CFLAGS) $(GOPT) $(GTEST) graphics.c -o graphics
$(G): $(GOBJS)
	$(CC) $(CFLAGS) $(GOPT) $(GOBJS) $(GTEST) -o $(G)

.PHONY: test clean

mazeGraph.o: mazeGraph.h
graphics.o: graphics.h
	$(CC) $(CFLAGS) $(GOPT) $(GTEST) graphics.c -o graphics.o


test: $(PROG)
	./$(PROG) 

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f $(LIB)
	rm -f $(G)
