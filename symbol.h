/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: symbol.h
 Notes: All method prototypes and structs for building and
    acessing SYMTAB.
 *************************************************************/

#ifndef symbol_h
#define symbol_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Symbol
{
    char label[7];
    int value; /* made this signed as value is not
                necessarily an address*/
    char type;
}Symbol;

typedef struct Literal
{
    char name[7];
    char literal[9];
    unsigned int length;
    unsigned int address;
}Literal;

void BuildTables(FILE *symfile, char *symfname);

int SymHeaderPresent(FILE *symfile);

int LitHeaderPresent(FILE *symfile);

void BuildSYMTAB(FILE *symfile, int size);

void BuildLITTAB(FILE *symfile, int size);

char *GetSymbolName(int address);

Literal *GetLiteral(unsigned int address);

#endif /* symbol_h */

/*********************[ EOF: symbol.h ]***********************/
