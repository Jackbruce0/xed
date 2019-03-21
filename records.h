/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: recordaa.h
 Notes: Contains struct definitions for each record type and
    prototypes for methods dealing with reading obj code and
    organizing inside of structs.
 *************************************************************/

#ifndef records_h
#define records_h

#include <stdio.h>

/***********************[ Record Types ]**********************/
typedef struct Header
{
    char* name;
    unsigned char startAddress[3];
    unsigned char programLength[3];
}Header;

typedef struct Text{
    unsigned char startAddress[3];
    int recLength;
    unsigned char inst[30];
}Text;

typedef struct Mod
{
    unsigned char startAddress[3];
    int modLength;//IN 1/2 Bytes
    //SYMBOL VALUE UNKNOWN(char*?)
}Mod;

typedef struct End
{
    unsigned char firstinst[3] ;//optional?
}End;
/******************[ End of Record Types ]********************/

int filesExist(const char* filename);

void doStuff(void);

#endif /* records_h */

/*********************[ EOF: records.h ]**********************/
