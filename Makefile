###########################################################
# Makefile for CS530 Assignment 2
# Names: Jack Bruce & Jacob Romio
# usernames: cssc0420 & cssc0413
###########################################################
OBJS = main.o format.o linkedlist.o optab.o records.o symbol.o xedlib.o
CC = gcc
LDFLAGS = -lm
CPP = -c

xed: $(OBJS)
	$(CC) $(OBJS) -o xed $(LDFLAGS)

main.o: main.c symbol.h xedlib.h
	$(CC) $(CPP) main.c

format.o: format.c instruction.h xedlib.h 
	$(CC) $(CPP) format.c

linkedlist.o: linkedlist.c xedlib.h
	$(CC) $(CPP) linkedlist.c

optab.o: optab.c optab.h
	$(CC) $(CPP) optab.c

records.o: records.c records.h xedlib.h
	$(CC) $(CPP) records.c

symbol.o: symbol.c symbol.h xedlib.h
	$(CC) $(CPP) symbol.c

xedlib.o: xedlib.c xedlib.h
	$(CC) $(CPP) xedlib.c

clean:
	rm *.o

#######################[ EOF: Makefile ]###################
