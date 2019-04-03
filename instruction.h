/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: instruction.h
 Notes: Contains struct definition for instruction type. This
    struct will be populated by methods in format.c
    Protptypes for other methods.
    TBA
 *************************************************************/

#ifndef instruction_h
#define instruction_h

#include <stdio.h>

typedef struct Instruction
{
    /* Column numbers refer to the columns of listing files
     and all extra space in char arrays is for formatting
     Count number of chars in each column in the listing
     file */
    unsigned int startadr; /* Col 1
                            6 hex digits (4 in example)
                            USE CharToNum to populate this
                            variable */
    char label[6]; /* Col 2
                    ALL SPACES IF NONE */
    char opname[7]; /* Col 3
                     if (format 4) [0] = '+'
                     else [0] = ' ' */
    char operand[9]; /* Col 4
                      if (immediate) [0] = '#'
                      if (indirect) [0] = '&'
                      if (literal) [0] = '='
                      else [0] = ' ' */
    unsigned char objcode[4]; /* Col 5
                               raw obj code for each line */
} Instruction;
/******************[ End of Instruction type ]********************/

//function prototypes here
void formatCall(unsigned int recLength, unsigned char inst[30]);
int bit(unsigned char curByte, int NX);
unsigned char asciiToHex(unsigned char c1, unsigned char c2);
unsigned char byteToHalfByte(unsigned char curByte, int NX);
void binaryPrint(unsigned char curByte);
#endif /* instruction_h */

/*******************[ EOF: instruction.h ]********************/
