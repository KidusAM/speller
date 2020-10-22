# Compilation flags
CC = gcc

LDLIBS = -lm

CFLAGS = -g -Wall

LDFLAGS = -g

#End of compilation flags

speller: speller.o dictionary.o

speller.o: dictionary.h

dictionary.o: dictionary.h

.PHONY: clean
clean:
	rm -f *.o a.out core speller

.PHONY: all
all: clean speller
