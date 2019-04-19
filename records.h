/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: records.h
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
    char name[7]; //[1-6] (ASCII form)
    unsigned int startadr; //[7-12] (numeric form)
    unsigned int prglen; //[13-18] (numeric form)
}Header;

typedef struct Text
{
    unsigned int startadr;
    unsigned int reclength;
    unsigned char inst[30];
}Text;

typedef struct Mod
{
    unsigned int startadr;
    unsigned int modLength;//IN 1/2 Bytes
    //SYMBOL VALUE UNKNOWN(char*?)
}Mod;

typedef struct End
{
    unsigned int firstinst ;//optional?
}End;
/******************[ End of Record Types ]********************/

void CountRecords(FILE *objfile);

int GetTcount(void);

int GetMcount(void);

Header *GetHeader(FILE *objfile, char *objfname);

Text *GetText(FILE *objfile, char *objfname);

Mod *GetMod(FILE *objfile, char* objfname);

End *GetEnd(FILE *objfile, char* objfname);

#endif /* records_h */

/*********************[ EOF: records.h ]**********************/
