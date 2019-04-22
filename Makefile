###########################################################
# Makefile for CS530 Assignment 2
# Names: Jack Bruce & Jacob Romio
# usernames: cssc0420 & cssc0413
###########################################################
#NEEDS TO BE CHANGED FOR PROJECT 2!
xed: main.o
	gcc main.o -o xbd

main.o: main.c main.h
	gcc -c main.c

clean:
	rm *.o

#######################[ EOF: Makefile ]###################
