/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 RedID's: 822320220 & 822843795
 Project: CS530 Assignment 2
 File: records.c
 Notes: This file contains all methods to read, classify, and
    store object code into designated structs.
 *************************************************************/

#include <stdlib.h>
#include "records.h"
#include "xedlib.h"

/*************************************************************
 function: CountRecords
 Notes: Reads obj code and counts number of text and mod
    records. Rewinds file pointer upon completion.
    Assists in memory allocation for record arrays
    WE ASSUME FILE POINTER IS POINTING TO FIRST CHAR UPON
    METHOD CALL.
 I/O: input paramaters: Pointer to obj file
      output: void
 *************************************************************/
int tcount, mcount;
void CountRecords(FILE *objfile)
{
    tcount = 0; mcount = 0;
    int i = 0;
    while(i++ < 19)
    {
        fgetc(objfile); /* We have to skip header section in
                         case program name contains a T or M */
    }
    char c;
    while((c = fgetc(objfile)) != EOF)
    {
        if(c == 'T')
        {
            tcount++;
        }
        if(c == 'M')
        {
            mcount++;
        }
    }
    rewind(objfile);
} /* End function Count_Records */

/* Getter functions for results of Count_Records */
int GetTcount()
{
    return tcount;
}
int GetMcount()
{
    return mcount;
}
/* End of getter functions */

/*************************************************************
 function: GetHeader
 Notes: Reads first line of obj file and interprets it as
    a header record, converts each char to numeric hex value
    (except for Name), and stores each digit into the
    appropriate field.
    WE ASSUME FILE POINTER IS POINTING TO FIRST CHAR UPON
    METHOD CALL.
 I/O: input paramaters: Pointer to obj file and obj file name
      output: pointer to populated Header struct (record)
 *************************************************************/
Header* GetHeader(FILE *objfile, char* objfname)
{
    Header *hptr = malloc(sizeof(Header));
    hptr->startadr = 0;
    hptr->prglen = 0;
    unsigned char c;
    if((c = fgetc(objfile)) != 'H')
    {
        fprintf(stderr, "Error in %s\nNo header record \
                found.\n", objfname);
        exit(1);
    }
    int filenx, namenx = 0, adrnx = 0, lennx = 0;
    for(filenx = 1; filenx < 19; filenx++)
    {
        c = fgetc(objfile);
        
        if(filenx < 7) /* progname */
        {
            hptr->name[namenx++] = c;
        }
        else if (filenx < 13) /* startadr */
        {
            hptr->startadr = CharToNum(hptr->startadr, c,
                                      adrnx++, 6);
        }
        else if (filenx < 19) /* prglen */
        {
            hptr->prglen = CharToNum(hptr->prglen, c,
                                     lennx++, 6);
        }
    }
	hptr->name[6] = '\0';
    fgetc(objfile); /* Burn '\n' at the end of
                     Header record */
    
//    fgetc(objfile);//needed to add a second one to work(Jake)
    
    return hptr;
} /* End function Get_Header */

/*************************************************************
 function: GetText
 Notes: Reads next line of obj file and interprets it as a
    Text record. Converts each char to numeric hex value
    and stores each digit into the appropriate field.
 I/O: input paramaters: Pointer to obj file and obj file name
      output: pointer to populated Text struct (record)
 *************************************************************/
Text* GetText(FILE *objfile, char* objfname)
{
    Text *tptr = malloc(sizeof(Text));
    tptr->startadr = 0;
    tptr->reclength = 0;
    int filenx = 0, adrnx = 0, lennx = 0, instnx = 0, bytepos;
    unsigned char c;
    while(filenx++ < 6) /*startadr*/
    {
        c = fgetc(objfile);
        tptr->startadr = CharToNum(tptr->startadr, c,
                                   adrnx++, 6);
    }
    while(filenx++ < 9) /*reclength*/
    {
        c = fgetc(objfile);
        tptr->reclength = CharToNum(tptr->reclength, c,
                                    lennx++, 2);
    }
    while(instnx < tptr->reclength)
    {
        tptr->inst[instnx] = 0;
        bytepos = 0;
        while(bytepos < 2)
        {
            c = fgetc(objfile);
            tptr->inst[instnx] = CharToNum(tptr->inst[instnx],
                                           c, bytepos++, 2);
        }
        instnx++;
    }
    fgetc(objfile); /* Burn '\n' at the end of
                     Text record */
//    fgetc(objfile);
    return tptr;
} /* End function Get_Text */

/*************************************************************
 function: GetMod
 Notes: Reads next line of obj file and interprets it as a
    Mod record. Converts each char to numeric hex value
    and stores each digit into the appropriate field.
 I/O: input paramaters: Pointer to obj file and obj file name
      output: pointer to populated Mod struct (record)
 *************************************************************/
Mod *GetMod(FILE *objfile, char* objfname)
{
    Mod *mptr = malloc(sizeof(Mod));
    mptr->startadr = 0;
    mptr->modLength = 0;
    
    int filenx = 0, adrnx = 0, lennx = 0;
    unsigned char c;
    while(filenx < 6) /*startadr*/
    {
        c = fgetc(objfile);
        mptr->startadr = CharToNum(mptr->startadr, c,
                                   adrnx++, 6);
        filenx++;
    }
    while(filenx < 8)
    {
        c = fgetc(objfile);
        mptr->modLength = CharToNum(mptr->modLength, c,
                                    lennx++, 2);
        filenx++;
    }
    fgetc(objfile); /* Burn '\n' at the end of
                     Mod record */
//    fgetc(objfile);
    return mptr;
} /* End function Get_Mod */

/*************************************************************
 function: GetEnd
 Notes: Reads next line of obj file and interprets it as a
    End record. Converts each char to numeric hex value
    and stores each digit into the appropriate field.
 I/O: input paramaters: Pointer to obj file and obj file name
      output: pointer to populated End struct (record)
 *************************************************************/
End *GetEnd(FILE *objfile, char* objfname)
{
    End *eptr = malloc(sizeof(End));
    eptr->firstinst = 0;
    
    int filenx = 0, adrnx = 0;
    char c;
    while(filenx < 6) /*firstinst*/
    {
        c = fgetc(objfile);
        eptr->firstinst = CharToNum(eptr->firstinst, c,
                                    adrnx++, 6);
        filenx++;
    }
    return eptr;
} /* End function Get_End */

/*********************[ EOF: records.c ]**********************/
