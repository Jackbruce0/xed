/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: main.c
 Notes: SIC XE Disassembler
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "records.h"

/*************************************************************
 function: FileOpen
 Notes: Opens given file in specified mode. Exits program
    if unsuccessful.
 I/O: input paramaters: base file name and extension name
    output: char* with value of baseName + ext
 *************************************************************/
FILE* FileOpen(char* fileName, char* mode)
{
    FILE* fPtr;
    fPtr = fopen(fileName, mode);
    if(fPtr == NULL)
    {
        fprintf(stderr, "Cannot open file: %s\n", fileName);
        exit(1);
    }
    return fPtr;
} /* End function File_Open */

/*************************************************************
 function: GetFileExt
 Notes: Combines a base file name with extension
 I/O: input paramaters: base file name and extension name
    output: char* with value of baseName + ext
 *************************************************************/
char* GetFileExt(char* baseName, char* ext)
{
    char* fileName = malloc((strlen(baseName)+4)*sizeof(char));
    strcpy(fileName, baseName);
    strcat(fileName, ext);
    return fileName;
} /* End function Get_File_Ext */

int main(int argc, const char * argv[])
{
    char* fname = (char*)argv[1];
    char* objfname = GetFileExt(fname, ".obj"); //fname.obj
    char* symfname = GetFileExt(fname, ".sym"); //fname.sym
    /*********** STEP 1 ***********/
    /* File Check/Open */
    FILE* objptr = FileOpen(objfname, "rb");
    //FILE* symptr = FileOpen(symfname, "rb");
    
    
    //Read in first 16 chars and store in Header
    
    /******** END OF STEP 1 *******/
    return 0;
}

/************************[ EOF: main.c ]**********************/
