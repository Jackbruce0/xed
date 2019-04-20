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
    char label[7]; /* Col 2
                    ALL SPACES IF NONE */
    char opname[8]; /* Col 3
                     if (format 4) [0] = '+'
                     else [0] = ' ' */
    char operand[10]; /* Col 4
                      if (immediate) [0] = '#'
                      if (indirect) [0] = '@'
                      if (literal) [0] = '='
                      else [0] = ' ' */
    unsigned char objcode[4]; /* Col 5
                               raw obj code for each line */
	int format;/*keeps track of format for locctr*/
} Instruction;

typedef struct node* link;

    struct node
	{
        Instruction* instptr;
		link next;
    };
/***************[ End of struct declarations ]****************/
/**********function prototypes************/

/*linkedlist.c prototypes*/
link Add(link head, Instruction *inst);
unsigned int GetLocctr(link head);
void PrintList(link head);
void FreeItems(link head);
void FreeList(link head);

/*format.c prototypes*/
link FormatCall(unsigned int reclength, 
				unsigned char inst[30], link head,
				unsigned int locctr);
int Bit(unsigned char curbyte, int bytenx);
unsigned char AsciiToHex(unsigned char c1, unsigned char c2);
unsigned char ByteToHalfByte(unsigned char curbyte, 
											int halfnx);
void BinaryPrint(unsigned char curbyte);
Instruction* OpcodeCopy(unsigned char curbyte, 
									Instruction* instptr);
link InsertRESDirectives(link HEAD, int LOCCTR, int nextaddr);
#endif /* instruction_h */

/*******************[ EOF: instruction.h ]********************/
