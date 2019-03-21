/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: records.c
 Notes: This file contains all methods to read, classify, and
 store object code into designated structs.
 *************************************************************/

#include <stdlib.h>
#include <math.h>
#include "records.h"

/*************************************************************
 function: GetHeader
 Notes: Reads first 16 chars of obj file, converts each char
    to numeric hex value (except for Name), and stores
    each digit into the appropriate field.
    WE ASSUME FILE POINTER IS UNUSED UPON METHOD CALL.
 I/O: input paramaters: Pointer to obj file
      output: pointer to populated Header struct (record)
 *************************************************************/
Header* GetHeader(FILE* objfile, char* objfname)
{
    
    Header* hptr = malloc(sizeof(Header));
    hptr->strtadr = 0;
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
        //printf("%c\n", c);
        if(filenx < 7) /* progname */
        {
            hptr->name[namenx++] = c;
        }
        else if (filenx < 13) /* stradr */
        {
            hptr->strtadr = CharToNum(hptr->strtadr, c,
                                      adrnx++, 6);
        }
        else if (filenx < 19) /* prglen */
        {
            hptr->prglen = CharToNum(hptr->prglen, c,
                                     lennx++, 6);
        }
    }
    
    return hptr;
} /* End function Get_Header */

/*************************************************************
 function: CharToNum (DECIMAL NUMBER)
 Notes: Takes character, interprets it as a hex digit, and
    adds value based on position to a destination integer.
    FOR POSITION: 0 = most significant place
 I/O: input paramaters: destination int, character,
    position, size
 output: dest int (incomplete until pos = size - 1
 *************************************************************/
int CharToNum(unsigned int dest, unsigned char input,
               int pos, int size)
{
    /* Store input's ASCII value (decimal) as an integer */
    unsigned int charnum = (unsigned int)input;
    
    /* Convert ASCII value to numeric value */
    if (charnum >= 48 && charnum <= 57) // [0-9]
    {
        charnum -= 48;
    }
    else if (charnum >= 55) // [A-F]
    {
        charnum -= 55; // only takes care of A-F
    }
    
    
    /* Add appropriate value to dest */
    int power = size - pos - 1;
    charnum = charnum * pow(16, power);
    
    /* Add value to dest and return */
    return dest += charnum;
} /* End function Char_To_Num */


/*********************[ EOF: records.c ]**********************/
