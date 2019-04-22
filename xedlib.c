/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 RedID's: 822320220 & 822843795
 Project: CS530 Assignment 2
 File: xedlib.c
 Notes: Library of general functions that are used throughout
 this project
 *************************************************************/

#include "xedlib.h"

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
    else if (charnum >= 65 && charnum <= 70) // [A-F]
    {
        charnum -= 55; // only takes care of A-F
    }
    else
    {
        fprintf(stderr, "Invalid value passed to 'CharToNum': '%c'\n", input);
        //exit(1);
    }
    /* Add appropriate value to dest */
    int power = size - pos - 1;
    charnum = charnum * pow(16, power);
    /* Add value to dest and return */
    return dest += charnum;
} /* End function Char_To_Num */

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

/*************************************************************
 function: Bit
 Notes: Takes a byte of data and an index to return that given
	bit within the byte.
 I/O: input paramaters: A byte and an index
      output: A bit within the byte, either 0 or 1.
 *************************************************************/

int Bit(unsigned char curbyte, int bytenx)
{
    curbyte >>= bytenx;
    int bitval = curbyte & 1;
    return bitval;
}

/*************************************************************
 function: AsciiToHex(Debugger)
 Notes: This takes in two ascii characters from the .obj file
	and compresses them into 1 byte of data.
 I/O: input paramaters: Two ascii characters read from the
	.obj file.
      output: 1 byte of data
 *************************************************************/

unsigned char AsciiToHex(unsigned char c1, unsigned char c2)
{
	//c1
	if('0' <= c1 && c1 <= '9')
	{
		c1-='0';
		c1<<=4;
	}
	else if('a' <= c1 && c1 <= 'f')
	{
		c1=c1+10-'a';
		c1<<=4;
	}
	else if('A' <= c1 && c1 <= 'F')
	{
		c1=c1+10-'A';
		c1<<=4;
	}
	else
	{
		printf("Non-hex value in the text record.\n");
		exit(1);
	}
	
	//c2
	if('0' <= c2 && c2 <= '9')
		c2-='0';
	else if('a' <= c2 && c2 <= 'f')
		c2=c2+10-'a';
	else if('A' <= c2 && c2 <= 'F')
		c2=c2+10-'A';
	else
	{
		printf("Non-hex value in the text record.\n");
		exit(1);
	}
	
	return c1+c2;
}

/*************************************************************
 function: ByteToHalfByte
 Notes: Takes a byte of data and an index for what half of the
	byte to return.
 I/O: input paramaters: A byte and an index, NX=0 or 1
      output: A single hex value as a half-byte
 *************************************************************/

 unsigned char ByteToHalfByte(unsigned char curbyte,
											int halfnx){
	unsigned char value;
	if(halfnx == 1)
		return value = (Bit(curbyte,7)*8 + Bit(curbyte,6)*4
	+ Bit(curbyte,5)*2 + Bit(curbyte,4));
	else
		return value = (Bit(curbyte,3)*8 + Bit(curbyte,2)*4
	+ Bit(curbyte,1)*2 + Bit(curbyte,0));
 }
 
/*************************************************************
 function: BinaryPrint(Debugger)
 Notes: Used for debugging purposes to print out the binary
	representation of a byte to confirm it's working as
	intended and to identify which bit is the bit to check.
 I/O: input paramaters: A byte of data
      output: Binary representation on terminal.
 *************************************************************/

void BinaryPrint(unsigned char curbyte)
{
	int j, bitval, ctr=0;
    for(j=7;j>=0;j--){
		bitval = Bit(curbyte, j);
        printf("%d", bitval);
        ctr++;
        if(ctr==4){
            printf(" ");
            ctr=0;
        }
    }
}
/**********************[ EOF: xedlib.h ]**********************/
