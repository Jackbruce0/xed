/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 RedID's: 822320220 & 822843795
 Project: CS530 Assignment 2
 File: symbol.c
 Notes: All methods for:
    1. reading <filename>.sym and classifying
    into tokens.
    2. building and accessing SYMTAB and LITTAB
 *************************************************************/

#include "symbol.h"
#include "xedlib.h"

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
        fprintf(stderr, "Header for .sym file must follow the exact format of the example you gave us (sample.sym)\n");
        exit(1);
    }
    char line[100];
    fgets(line, 99, symfile); /* Burn line of _'s */
    while(line[0] != '\n')
    {
        fgets(line, 99, symfile);
        if(line[0] == '\n') break;
        symcount++;
    }
    printf("Symbol Count = %d\n", symcount);
    if(!LitHeaderPresent(symfile)) /* cursor is now on next
                                    line */
    {
        fprintf(stderr, "Invalid header for LITTAB: %s\n",
                symfname);
        fprintf(stderr, "Header for .sym file must follow the exact format of the example you gave us (sample.sym)\n");
        exit(1);
    }
    fgets(line, 99, symfile); /* Burn line of _'s */

    while(!feof(symfile))
    {
        if(!fgets(line, 99, symfile)) {
            break;
        }
        litcount++;
    }
    printf("Literal Count = %d\n\n", litcount);
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
    SYMTAB = malloc((size + 1)*sizeof(Symbol*));
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
        i++;
    }
    SYMTAB[size] = NULL;
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
    LITTAB = malloc((size + 1)*sizeof(Literal*));
    char line[100];
    fgets(line, 99, symfile); /* Burn 1st 3 lines after last
                               SYMTAB value*/
    fgets(line, 99, symfile);
    fgets(line,99, symfile);
    char token[10];
    int i = 0, j;
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
        LITTAB[i]->length = CharToNum(LITTAB[i]->length,
                                      line[19], 0, 1);
        /* 24 - 29 = Address */
        strncpy(token, SplitString(line,24, 29), 7);
        j = 0;
        while(j < 6)
        {
            LITTAB[i]->address = CharToNum(LITTAB[i]->address, token[j], j, 6);
            j++;
        }
        i++;
    }
    LITTAB[size] = NULL;
}

/*************************************************************
 function: GetSymbolName
 Notes: When given a value, this function will search SYMTAB
    and return char* of the appropriate symbol name. If no
    symbol is found this function will return whitespace.
 I/O: input paramaters: value of potential symbol
      output: label associated with that value (if it exists)
 *************************************************************/
char *GetSymbolName(int value)
{
    char *label = malloc(7);
    strncpy(label, "      \0", 7);
    int index = 0;
    while(SYMTAB[index] != NULL)
    {
        if(value == SYMTAB[index]->value)
        {
            strncpy(label,SYMTAB[index]->label,7);
            break;
        }
        index++;
    }
    return label;
} /* End of function Get_Symbol_Name */

/*************************************************************
 function: NextSymbolAddress
 Notes: When given a value, this function will search SYMTAB
   and return value of the next symbol in SYMTAB.
 I/O: input paramaters: value of potential symbol
      output: returns value of next
 *************************************************************/
int NextSymbolAddress(int value)
{
    int index = 0;
    while(SYMTAB[index] != NULL)
    {
        if(value == SYMTAB[index]->value)
        {
            if (SYMTAB[index + 1] != NULL)
                return SYMTAB[index + 1]->value;
            else
                return -1;
        }
        index++;
    }
    return -1;
} /* End of function Next_Symbol_Address */

/*************************************************************
 function: GetLastSymbolAddress
 Notes: Return value of last (occupied) entry of SYMTAB
 I/O: input paramaters: void
      output: value of last SYMBOL
 *************************************************************/
int GetLastSymbolAddress(void)
{
    int index = -1; /*offset b/c last element is NULL */
    while(SYMTAB[index + 1] != NULL) index++;
    return SYMTAB[index]->value;
} /* End of function Get_Last_Symbol_Address */

/*************************************************************
 function: GetLiteral
 Notes: When given a value, this function will search LITTAB
    and return Literal* of the appropriate literal name. If no
    literal is return value will POINT to NULL
 I/O: input paramaters: address of potential literal
      output: COPY of literal pointer (if found)
 *************************************************************/
Literal *GetLiteral(unsigned int address)
{
    Literal* lit = NULL;
    int index = 0;
    while(LITTAB[index] != NULL)
    {
        if(address == LITTAB[index]->address)
        {
            lit = malloc(sizeof(Literal));
            strncpy(lit->name, LITTAB[index]->name, 7);
            strncpy(lit->literal, LITTAB[index]->literal, 9);
            lit->address = LITTAB[index]->address;
            lit->length = LITTAB[index]->length;
            break;
        }
        index++;
    }
    return lit;
}

/*********************[ EOF: symbol.h ]***********************/
