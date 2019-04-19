/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: xedlib.c
 Notes: Library of general functions that are used throughout
 this project
 *************************************************************/

#include "xedlib.h"

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

/**********************[ EOF: xedlib.h ]**********************/
