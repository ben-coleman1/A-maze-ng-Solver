# Main makefile for an amazing project (you'll see)
# 
# CS50 W18 Group 5

PROG = AMStartup

all: 
	$(MAKE) -C datastruct
	$(MAKE) -C src

.PHONY: test clean

test: all
	./src/AMStartup -n 2 -d 0 -h flume.cs.dartmouth.edu 


clean:
	rm -f *~
	rm -f $(PROG)
	rm -f Amazing_*
	$(MAKE) -C datastruct clean
	$(MAKE) -C src clean
