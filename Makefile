EXE = p-counter
CFLAGS =-O -g -Wall
CXXFLAGS = -Wall
LDLIBS =
CC = gcc
CXX = g++

.PHONY: all
all: $(EXE)

# Implicit rules defined by Make, but you can redefine if needed
#
p-counter: p-counter.c
	$(CC) $(CFLAGS) p-counter.c -o p-counter

testing: testing.c
	$(CC) $(CFLAGS) testing.c -o t
#
# OR
#
#p-counter: p-counter.cc
#	$(CXX) $(CXXFLAGS) p-counter.cc $(LDLIBS) -o p-counter

.PHONY: clean
clean:
	rm  p-counter t
