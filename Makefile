###########################################################
# Makefile for CS530 Assignment 2
# Names: Jack Bruce & Jacob Romio
# usernames: cssc0420 & cssc0413
###########################################################
xed: main.o
	gcc *.o -o xed

main.o: *.c *.h
	gcc -c *.c -lm

clean:
	rm *.o

#######################[ EOF: Makefile ]###################
