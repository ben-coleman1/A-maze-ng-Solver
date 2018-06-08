# Makefile for an amazing project (you'll see)
# 
# CS50 W18 Group 5

L1 = ../datastruct
PROG = AMStartup
OBJS = AMStartup.o message.o simpleAvatar.o amazing_client.o
LLIBS = $(L1)/datastruct.a
all:
	make -C $(L1)
	make $(PROG)


# uncomment the following to turn on verbose memory logging
DISP = -DDISPLAY `pkg-config --cflags gtk+-2.0 --libs`

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(L1) $(DISP) -pthread $(DISP)
CC = gcc
MAKE = make


$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o ../$(PROG)

.PHONY: test clean

test: $(PROG)
	./AMStartup -n 2 -d 0 -h flume.cs.dartmouth.edu 

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	##rm -f Amazing_*
	$(MAKE) -C $(L1)  clean
