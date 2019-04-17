/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: symbol.c
 Notes: All methods for:
    1. reading <filename>.sym and classifying
    into tokens.
    2. building and accessing SYMTAB and LITTAB
 *************************************************************/

#include "symbol.h"
#include "records.h" /* Spaghetti code?
                        We should put char to num
                        in another place*/

Symbol **SYMTAB;
Literal **LITTAB;

/*************************************************************
 function: BuildTables
 Notes: Performs first pass of <filename>.sym
    Gets counts for symbols and literals so BuildSYMTAB and
    BuildLITTAB can have properly sized arrays
 I/O: input paramaters: pointer to <filename>.sym
      output: void
 *************************************************************/
void BuildTables(FILE *symfile, char *symfname)
{
    int symcount = 0, litcount = 0;
    if(!SymHeaderPresent(symfile)) /* cursor is now on next
                                    line */
    {
        fprintf(stderr, "Invalid header for SYMTAB: %s\n",
                symfname);
        exit(1);
    }
    printf("%s\n", "SYMTAB checks out to me!");
    char line[100];
    fgets(line, 99, symfile); /* Burn line of _'s */
    printf("%s", line);
    while(line[0] != '\n')
    {
        fgets(line, 99, symfile);
        if(line[0] == '\n') break;
        printf("%s", line);
        symcount++;
    }
    printf("symbol count = %d\n\n", symcount);
    if(!LitHeaderPresent(symfile)) /* cursor is now on next
                                    line */
    {
        fprintf(stderr, "Invalid header for LITTAB: %s\n",
                symfname);
        exit(1);
    }
    printf("%s\n", "LITTAB checks out to me!");
    fgets(line, 99, symfile); /* Burn line of _'s */
    printf("%s", line);
    while(!feof(symfile))
    {
        if(!fgets(line, 99, symfile)) {
            printf("End of file!\n");
            break;
        }
        printf("%s", line);
        litcount++;
    }
    printf("literal count = %d\n\n", litcount);
    rewind(symfile);
    BuildSYMTAB(symfile, symcount);
    //BuildLITTAB(symfile, litcount);
    
    
} /* End function Build_Tables */

/*************************************************************
 function: SymHeaderPresent
 Notes: Checks if first line of <filename>.sym is the proper
    header for a symtab.
    I/O: input paramaters: pointer to <filename>.sym
         output: int (for Boolean use)
 *************************************************************/
int SymHeaderPresent(FILE *symfile)
{
    char expectedH[24] = "Symbol  Value   Flags:\n";
    char actualH[100];
    fgets(actualH, 100, symfile);
    return (strncmp(actualH, expectedH, 24) == 0);
} /* End function Sym_Header_Present */

/*************************************************************
 function: LitHeaderPresent
 Notes: Checks if first line of <filename>.sym is the proper
 header for a symtab.
 I/O: input paramaters: pointer to <filename>.sym
 output: int (for Boolean use)
 *************************************************************/
int LitHeaderPresent(FILE *symfile)
{
    char expectedH[34] = "Name    Literal  Length Address:\n";
    char actualH[100];
    fgets(actualH, 100, symfile);
    return (strncmp(actualH, expectedH, 34) == 0);
} /* End function Lit_Header_Present */

/*************************************************************
 function: BuildSYMTAB
 Notes: Assumes SYMTAB header is present and file cursor is
    placed at the beginning of next line.
    Reads <filename>.sym, tokenizes input into symbol structs,
    and places them into an array that will serve as our SYMTAB
 I/O: input paramaters: pointer to <filename>.sym
      output: void
 *************************************************************/
void BuildSYMTAB(FILE *symfile, int size) {
    SYMTAB = malloc(size*sizeof(Symbol*));
    char line[100];
    fgets(line, 99, symfile); /* Burn 1st 2 lines of file */
    fgets(line, 99, symfile);
    char token[7];
    int i = 0, j;
    while(i < size)
    {
        SYMTAB[i] = malloc(sizeof(Symbol));
        SYMTAB[i]->value = 0;
        fgets(line, 99, symfile);
        /* 0 - 5 = label*/
        strncpy(token, strtok(line, "  "), 6); /* Would like to delimit with ANY digit
                                                as opposed to spaces (for format)*/
        strncpy(SYMTAB[i]->label, token, 6);
        /* 8 - 13 = value*/
        strncpy(token, strtok(NULL, "  "), 6); /* Get next token */
        j = 0;
        while(j < 6)
        {
            SYMTAB[i]->value = CharToNum(SYMTAB[i]->value,
                                         token[j], j, 6);
            j++;
        }
        /* 26 = type */
        strncpy(token, strtok(NULL, "  "), 6); /* Get next token */
        SYMTAB[i]->type = token[0];
        
        /* TEST */
        printf("This is from the SYMTAB data structure!\n");
        printf("%s  %06X  %c\n", SYMTAB[i]->label, SYMTAB[i]->value, SYMTAB[i]->type);
        /********/
        
        i++;
    }
} /* End function Build_SYMTAB */

/*********************[ EOF: symbol.h ]***********************/
