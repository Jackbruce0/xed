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
#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "records.h"

FILE *FileOpen(char *fileName, char *mode);

char *GetFileExt(char *baseName, char *ext);

void FreeMem(char* objfname, char* symfname,
			Header* H, Text** T, Mod** M, End* E, link head);

int CharToNum(unsigned int dest, unsigned char input,
              int pos, int size);

char *SplitString(char *str, int startNX, int endNX);

int Bit(unsigned char curbyte, int bytenx);

unsigned char AsciiToHex(unsigned char c1, unsigned char c2);

unsigned char ByteToHalfByte(unsigned char curbyte, 
											int halfnx);
											
void BinaryPrint(unsigned char curbyte);

#endif /* xedlib_h */

/**********************[ EOF: xedlib.h ]**********************/
