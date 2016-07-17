include ../Makefile.incl

LIB = 
INC = -I$(HOME)/lib/boost/ -I./inc

all: Coloring

Coloring:
	cd src && $(MAKE) $@

clean:
	cd src && $(MAKE) $@
	cd inc && $(MAKE) $@
	rm -f Coloring *~

.PHONY:	Coloring clean doc
