/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: main.c
 Notes: SIC XE Disassembler
    SYNOPSIS:
        xed <filename>
            - filename will not include any extensions
            - filename must contain path to directory that
                contains <filename>.obj and <filename>.sym
            - ex) xed ./test/sample
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "records.h"
#include "instruction.h"
#include "symbol.h"

/*************************************************************
 I DON'T KNOW WHERE TO PUT THESE PROTOTYPES
 (apprently main.h is a wierd thing to do)
*************************************************************/
FILE *FileOpen(char *fileName, char *mode);
char *GetFileExt(char *baseName, char *ext);
/* End of main.c prototypes */

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
    char *fileName = malloc((strlen(baseName)+5)*sizeof(char));
    strncpy(fileName, baseName, strlen(baseName) + 1);
    strncat(fileName, ext, 4);
    return fileName;
} /* End function Get_File_Ext */

/*************************************************************
 function: FreeMem
 Notes: Frees memory reserved by malloc.
 I/O: input paramaters: each pointer for malloc calls
      output: none
 *************************************************************/
void FreeMem(char* objfname, char* symfname,
			Header* H, Text** T, Mod** M, End* E, link head)
{
    free(objfname);
	free(symfname);
	int i;
	for(i=0; i<GetTcount(); i++)
	{
		free(T[i]);
	}
	for(i=0; i<GetMcount(); i++)
	{
		free(M[i]);
	}
	free(T);
	free(M);
	free(H);
	free(E);
	FreeItems(head);
	FreeList(head);
} /* End function FreeMem */

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
    Header *H = GetHeader(objfile, objfname);
    
    int textnx = 0, modnx = 0;
    char c;
    while((c=fgetc(objfile)) != 'E')
    {
		if(c == 'T')
        {
            T[textnx++] = GetText(objfile, objfname);
        }
        else if(c == 'M')
        {
            M[modnx++] = GetMod(objfile, objfname);
        }
        else
        {
            fprintf(stderr, "Error in %s\n'%c' record is \
                    unknown\n", objfname, c);
            exit(1);
        }
    }
    End *E = GetEnd(objfile, objfname);
    
    /* SYM and LIT table processing */
    BuildTables(symfile, symfname);
    /********************************/
    
    fclose(objfile);
    fclose(symfile);
    /******** END OF STEP 1 *******/
	
	/*********** STEP 2 ***********/
	link head = NULL;
	for(textnx=0; textnx<GetTcount(); textnx++)
	{
		head=FormatCall(T[textnx]->reclength,
						T[textnx]->inst, head,
						T[textnx]->startadr);
	}
	/******** END OF STEP 2 *******/
    
    //test//
    printf("T record count: %d, M record count: %d\n",
           GetTcount(), GetMcount());
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
    printf("\n\n");
//    printf("1st mod record start address: %06X\n",
//           M[0]->startadr);
//    printf("1st mod record length: %02X\n\n",
//           M[0]->modLength);
    printf("Address of 1st executable instruction: %06X\n",
           E->firstinst);
	printf("\nSOURCE CODE DISASSEMBLEMENT PROGRESS\n");
	PrintList(head);
    ///////
	FreeMem(objfname, symfname, H, T, M, E, head);
    return 0;
}

/************************[ EOF: main.c ]**********************/
