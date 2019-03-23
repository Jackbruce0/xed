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
FILE *FileOpen(char *fileName, char *mode)
{
    FILE *fPtr;
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
char *GetFileExt(char *baseName, char *ext)
{
    char *fileName = malloc((strlen(baseName)+4)*sizeof(char));
    strcpy(fileName, baseName);
    strcat(fileName, ext);
    return fileName;
} /* End function Get_File_Ext */

int main(int argc, const char * argv[])
{
    char *fname = (char*)argv[1];
    char *objfname = GetFileExt(fname, ".obj"); //fname.obj
    char *symfname = GetFileExt(fname, ".sym"); //fname.sym
    
    /*********** STEP 1 ***********/
    /* File Check/Open */
    FILE *objfile = FileOpen(objfname, "rb");
    FILE *symfile = FileOpen(symfname, "rb");
    
    CountRecords(objfile);
    Text **T = malloc(GetTcount()*sizeof(Text*));
    Mod **M = malloc(GetMcount()*sizeof(Mod*));
    Header* H = GetHeader(objfile, objfname);
    
    int textnx = 0, modnx = 0;
    char c;
    while((c=fgetc(objfile)) != 'E')
    {
        if(c == 'T')
        {
            T[textnx] = GetText(objfile, objfname);
        }
        else if(c == 'M')
        {
            
        }
        else
        {
//            fprintf(stderr, "Error in %s\n'%c' record is \
//                    unkown\n", objfname, c);
//            exit(1);
        }
    }
    
    fclose(objfile);
    fclose(symfile);
    /******** END OF STEP 1 *******/
    
    //test//
    printf("T count: %d, M count: %d\n", GetTcount(),
           GetMcount());
    printf("Program name: %s\n", H->name);
    printf("Starting address: %06X\n", H->startadr);
    printf("Program length: %06X\n\n", H->prglen);
    printf("1st text record start address: %06X\n",
           T[0]->startadr);
    printf("1st text record length: %02X\n", T[0]->reclength);
    printf("Instructions: ");
    int i = 0;
    while(i < T[0]->reclength) {
        printf("%02X ", T[0]->inst[i]);
        i++;
    }
    printf("\n");
    ///////
    return 0;
}

/************************[ EOF: main.c ]**********************/
