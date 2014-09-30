###########################################################################3
#
# Makefile
# Copyright (C) 2014 Carnegie Mellon University
#
# 18-640 Fall 2014 Project 2 Makefile
#
#


GIVEN_OBJECTS = tomasulo_sim.o
OBJECTS = tomasulo.o
SOURCES = tomasulo.c 
HEADERS = tomasulo.h 
EXECFILE = tomasulo_sim 
 
$(EXECFILE): $(OBJECTS) $(GIVEN_OBJECTS)
	gcc $(OBJECTS) $(GIVEN_OBJECTS) -o $(EXECFILE)
$(OBJECTS): $(SOURCES) $(HEADERS)
	gcc -g -c $(SOURCES)
clean:
	rm -rf $(OBJECTS) $(EXECFILE) 
