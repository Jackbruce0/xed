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
    char name[6]; //[1-6] (ASCII form)
    unsigned int strtadr; //[7-12] (numeric form)
    unsigned int prglen; //[13-18] (numeric form)
}Header;

typedef struct Text{
    unsigned int strtadr;
    unsigned int recLength;
    unsigned char inst[30]; /* this will be tricky to large
                             for numeric from */
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

Header* GetHeader(FILE* objfile, char* objfname);

int CharToNum(unsigned int dest, unsigned char input,
               int pos, int size);

#endif /* records_h */

/*********************[ EOF: records.h ]**********************/
