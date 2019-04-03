/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: format.c
 Notes: Breaks down contents of Text Records instructions to
	calculate values for each format type.
 *************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"
#include "optab.h"

enum boolean {false, true};

/*************************************************************
 function: formatCall
 Notes: 
 I/O: input paramaters: 
      output: 
 *************************************************************/

void formatCall(unsigned int recLength, 
								unsigned char inst[30]){
    unsigned char curByte, c1, c2, opCode;
    int i;
	
    for(i=0; i<recLength; i++)
	{
		Instruction* instptr = malloc(sizeof(instptr));
		curByte = inst[i];
		
		if(sicInstCheck(curByte)==1){//format 1
			printf("Format 1: %6s ", sicInstMnemonic(curByte));
			binaryPrint(inst[i]);
		}
		else if(sicInstCheck(curByte)==2){//format 2
			printf("Format 2: %6s ", sicInstMnemonic(curByte));
			unsigned char r1 = byteToHalfByte(inst[i+1], 1);
			unsigned char r2 = byteToHalfByte(inst[i+1], 0);
			binaryPrint(inst[i]);
			binaryPrint(inst[i+1]);
			printf("				r1 %01X,	r2 %01X", r1, r2);
			i++;
		}
		else{//format 0/3/4
			//check right-most bits of first byte so see if format 0
			if( bit(curByte,1)==0 && bit(curByte,0)==0 )//format 0
			{
				printf("Format 0: %6s ", sicInstMnemonic(curByte));
				unsigned int disp1 = (bit(inst[i+1],6)<<14) + (bit(inst[i+1],5)<<13) + (bit(inst[i+1],4)<<12);
				unsigned int disp2 = byteToHalfByte(inst[i+1], 0) << 8;
				unsigned char disp3 = inst[i+2];
				enum boolean xFlag=false;
				if(bit(inst[i+1],7)==1)
					xFlag=true;
				binaryPrint(inst[i]);
				binaryPrint(inst[i+1]);
				binaryPrint(inst[i+2]);
				unsigned int disp = disp1 + disp2 + disp3;
				printf("			x %1d	disp	%03X", xFlag, disp);
				i+=2;
			}
			//have a method to grab 2nd byte to check if format 3 or 4
			else
			{
				if(bit(inst[i+1],4)==0)//format 3
				{
					printf("Format 3: %6s ", sicInstMnemonic(curByte));
					unsigned int disp1 = byteToHalfByte(inst[i+1], 0) << 8;
					unsigned char disp2 = inst[i+2];
					enum boolean xFlag=false;
					char* addressingMode;
					if(bit(inst[i+1],7)==1)
						xFlag=true;
					if(bit(curByte,1)==1&&bit(curByte,0)==0)
						addressingMode = "Indirect";
					else if(bit(curByte,1)==0&&bit(curByte,0)==1)
						addressingMode = "Immediate";
					else
					{
						if(bit(inst[i+1],5)==1)
							addressingMode = "PC Relative";
						else
							addressingMode = "Base Relative";
					}
					binaryPrint(inst[i]);
					binaryPrint(inst[i+1]);
					binaryPrint(inst[i+2]);
					unsigned int disp = disp1 + disp2;
					printf("			x %1d	disp	%03X    %s", xFlag, disp, addressingMode);
					i+=2;
				}
				else//format 4
				{
					printf("Format 4: %6s ", sicInstMnemonic(curByte));
					unsigned int address1 = byteToHalfByte(inst[i+1], 0) << 16;
					unsigned int address2 = inst[i+2] << 8;
					unsigned char address3 = inst[i+3];
					enum boolean xFlag=false;
					char* addressingMode;
					if(bit(inst[i+1],7)==1)
						xFlag=true;
					if(bit(curByte,1)==1&&bit(curByte,0)==0)
						addressingMode = "Indirect";
					else if(bit(curByte,1)==0&&bit(curByte,0)==1)
						addressingMode = "Immediate";
					else
					{
						addressingMode = "Direct";
					}
					binaryPrint(inst[i]);
					binaryPrint(inst[i+1]);
					binaryPrint(inst[i+2]);
					binaryPrint(inst[i+3]);
					unsigned int address = address1 + address2 + address3;
					printf("	x %1d	adrs	%05X  %s", xFlag, address, addressingMode);
					i+=3;
				}
			}
		}
		printf("\n");
		free(instptr);
    }
}

/*************************************************************
 function: bit
 Notes: Takes a byte of data and an index to return that given
	bit within the byte.
 I/O: input paramaters: A byte and an index
      output: A bit within the byte, either 0 or 1.
 *************************************************************/

int bit(unsigned char curByte, int NX)
{
    curByte >>= NX;
    int bitVal = curByte & 1;
    return bitVal;
}

/*************************************************************
 function: asciiToHex
 Notes: This takes in two ascii characters from the .obj file
	and compresses them into 1 byte of data.
 I/O: input paramaters: Two ascii characters read from the
	.obj file.
      output: 1 byte of data
 *************************************************************/

unsigned char asciiToHex(unsigned char c1, unsigned char c2)
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
		printf("There exists a non-hex value in the text record.\n");
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
		printf("There exists a non-hex value in the text record.\n");
		exit(1);
	}
	
	return c1+c2;
}

/*************************************************************
 function: byteToHalfByte
 Notes: Takes a byte of data and an index for what half of the
	byte to return.
 I/O: input paramaters: A byte and an index, NX=0 or 1
      output: A single hex value as a half-byte
 *************************************************************/

 unsigned char byteToHalfByte(unsigned char curByte, int NX){
	unsigned char value;
	if(NX == 1)
		return value = (bit(curByte,7)*8 + bit(curByte,6)*4 + bit(curByte,5)*2 + bit(curByte,4));
	else
		return value = (bit(curByte,3)*8 + bit(curByte,2)*4 + bit(curByte,1)*2 + bit(curByte,0));
 }
 
/*************************************************************
 function: binaryPrint
 Notes: Used for debugging purposes to print out the binary
	representation of a byte to confirm it's working as
	intended and to identify which bit is the bit to check.
 I/O: input paramaters: A byte of data
      output: Binary representation on terminal.
 *************************************************************/

void binaryPrint(unsigned char curByte){
	int j, bitVal, ctr=0;
    for(j=7;j>=0;j--){
		bitVal = bit(curByte, j);
        printf("%d", bitVal);
        ctr++;
        if(ctr==4){
            printf(" ");
            ctr=0;
        }
    }
}
/*******************[ EOF: format.c ]********************/