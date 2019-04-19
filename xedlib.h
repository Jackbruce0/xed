/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: xedlib.h
 Notes: Library of general functions that are used throughout
    this project (Protoypes only)
 *************************************************************/

#ifndef xedlib_h
#define xedlib_h

#include <stdio.h>
#include <math.h>

int CharToNum(unsigned int dest, unsigned char input,
              int pos, int size);

char *SplitString(char *str, int startNX, int endNX);

#endif /* xedlib_h */

/**********************[ EOF: xedlib.h ]**********************/
