/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: optab.h
 Notes: Contains all of the mnemonics for the SIC/XE machine
	and can find each 
 *************************************************************/

#ifndef optab_h
#define optab_h

#include <stdio.h>

typedef struct Op
{
    /* Column numbers refer to the columns of listing files
     and all extra space in char arrays is for formatting
     Count number of chars in each column in the listing
     file */
    unsigned char opcode; /* Col 1
                            6 hex digits (4 in example)
                            USE CharToNum to populate this
                            variable */
    char mnemonic[6]; /* Col 2
                    ALL SPACES IF NONE */
    unsigned char format; /* Col 3
                     if (format 4) [0] = '+'
                     else [0] = ' ' */
} Op;

#endif /* optab_h */

/**prototypes**/
int SicInstCheck(unsigned char curbyte);
char* SicInstMnemonic(unsigned char curbyte);
char* RegisterTab(unsigned int regnum);
int NumOfRegisters(unsigned char curbyte);

/*******************[ EOF: optab.h ]********************/
