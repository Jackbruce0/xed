/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 RedID's: 822320220 & 822843795
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
#include "symbol.h"
#include "xedlib.h"

int main(int argc, const char * argv[])
{
    if(argv[1]==NULL)
	{
		printf("Please provide a <filename> that has ");
		printf("<filename>.obj and <filename>.sym ");
		printf("files to associate with.\n");
		exit(1);
	}
	char *fname = (char*)argv[1];
    char *objfname = GetFileExt(fname, ".obj"); //fname.obj
    char *symfname = GetFileExt(fname, ".sym"); //fname.sym
	char *sicfname = GetFileExt(fname, ".sic"); //fname.sic
	char *lisfname = GetFileExt(fname, ".lis"); //fname.lis
    
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
    int LOCCTR = 0;
    int nextaddr = 0; /* address of next instruction */
    int lastbyte = H->startadr + H->prglen;
	link head = NULL;
	head = InsertSTARTDirective(head, H->name, H->startadr);
	for(textnx=0; textnx<GetTcount(); textnx++)
	{
        LOCCTR = T[textnx]->startadr;
        if(textnx + 1 < GetTcount())
            nextaddr = T[textnx + 1]->startadr;
        else
            nextaddr = lastbyte; /* EOF case */
		head=FormatCall(T[textnx]->reclength,
						T[textnx]->inst, head,
						LOCCTR, lastbyte);
        LOCCTR += T[textnx]->reclength;
        head = InsertRESDirectives(head, LOCCTR, nextaddr);
	}
	head = InsertENDDirective(head, H->startadr);
	/******** END OF STEP 2 *******/
    
    /*********** STEP 3 ***********/
    printf("Program name: %s\n", H->name);
    printf("Starting address: %06X\n", H->startadr);
    printf("Program length: %06X\n\n", H->prglen);
	printf("\nSOURCE CODE DISASSEMBLEMENT\n");
	PrintList(head);
	WriteFile(head, sicfname, 0);
	WriteFile(head, lisfname, 1);
	/******** END OF STEP 3 *******/
	
	printf("\nFiles %s and %s were created.\n\n",
				sicfname, lisfname);
	FreeMem(objfname, symfname, H, T, M, E, head);
    return 0;
}

/************************[ EOF: main.c ]**********************/
