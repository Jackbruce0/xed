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
/* CharToNum and StringSplit should go to a general purpose
header file */

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
    BuildLITTAB(symfile, litcount);
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
    placed at the beginning of the file.
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
    printf("This is from the SYMTAB data structure!\n");
    while(i < size)
    {
        SYMTAB[i] = malloc(sizeof(Symbol));
        SYMTAB[i]->value = 0;
        fgets(line, 99, symfile);
        /* 0 - 5 = label */
        strncpy(token, SplitString(line, 0, 5), 7);
        strncpy(SYMTAB[i]->label, token, 7);
        /* 8 - 13 = value*/
        strncpy(token, SplitString(line, 8, 13), 7); /* Get next token */
        j = 0;
        while(j < 6)
        {
            SYMTAB[i]->value = CharToNum(SYMTAB[i]->value,
                                         token[j], j, 6);
            j++;
        }
        /* 16 = type */
        SYMTAB[i]->type = line[16];
        
        /* TEST */
        printf("%s  %06X  %c\n", SYMTAB[i]->label, SYMTAB[i]->value, SYMTAB[i]->type);
        /********/
        
        i++;
    }
} /* End function Build_SYMTAB */

/*************************************************************
 function: BuildLITTAB
 Notes: Assumes LITTAB header is present and file cursor is
    placed at the last line of SYMTAB.
    Reads <filename>.sym, tokenizes input into literal structs,
    and places them into an array that will serve as our LITTAB
 I/O: input paramaters: pointer to <filename>.sym
      output: void
 *************************************************************/
void BuildLITTAB(FILE *symfile, int size)
{
    LITTAB = malloc(size*sizeof(Literal*));
    char line[100];
    fgets(line, 99, symfile); /* Burn 1st 3 lines after last
                               SYMTAB value*/
    fgets(line, 99, symfile);
    fgets(line,99, symfile);
    printf("%s\n",line);
    char token[10];
    int i = 0, j;
    printf("This is from the LITTAB data structure!\n");
    while(i < size)
    {
        LITTAB[i] = malloc(sizeof(Literal));
        LITTAB[i]->length = 0;
        LITTAB[i]->address = 0;
        fgets(line,99, symfile);
        /* 0 - 5 = name */
        strncpy(token, SplitString(line, 0, 5), 7);
        strncpy(LITTAB[i]->name, token, 7);
        /* 8 - 16 = Literal */
        strncpy(token, SplitString(line,8, 16), 9);
        strncpy(LITTAB[i]->literal, token, 9);
        /* 19 = length */
        LITTAB[i]->length = CharToNum(LITTAB[i]->length, line[19], 0, 1);
        /* 24 - 29 = Address */
        strncpy(token, SplitString(line,24, 29), 7);
        j = 0;
        while(j < 6)
        {
            LITTAB[i]->address = CharToNum(LITTAB[i]->address, token[j], j, 6);
            j++;
        }
        
        /* TEST */
        printf("%s|  |%s|  |%X|  |%06X|\n", LITTAB[i]->name, LITTAB[i]->literal, LITTAB[i]->length, LITTAB[i]->address);
        /********/
        
        i++;
    }
}


/*************************************************************
 function: SplitString
 Notes: Returns substring specified by startNX and endNX
    DOES NOT CHECK FOR INVALID INDICES AND VALID SUBSTRING
    LENGTH
I/O: input paramaters: str, startNX, endNX
     output: substring fromm str[startNX] - str[endNX]
 *************************************************************/
char *SplitString(char *str, int startNX, int endNX)
{
    /* 30 char max substring including \0 */
    char* substr = malloc(30);
    int i = 0;
    while(startNX <= endNX)
    {
        substr[i] = str[startNX];
        i++;
        startNX++;
    }
    substr[i] = '\0';
    return substr;
} /* End of function Split_String */



/*********************[ EOF: symbol.h ]***********************/
