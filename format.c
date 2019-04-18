/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: format.c
 Notes: Breaks down contents of Text Records instructions to
	calculate  for each format type.
 *************************************************************/
#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "optab.h"

enum boolean {false, true};

/*************************************************************
 function: FormatCall(!NEEDS CLEANING!)
 Notes: Fills Instruction struct linked list
 I/O: input paramaters: reclength of Text Record,inst
						array holding contents, and head.
      output: Returns Head of linked list
 *************************************************************/

link FormatCall(unsigned int reclength, 
				unsigned char inst[30], link head,
				unsigned int locctr)
{
    unsigned char curbyte, c1, c2;
    int i;
	enum boolean baseflag = false;
	
    for(i=0; i<reclength; i++)
	{
		Instruction* instptr = malloc(sizeof(Instruction));
		curbyte = inst[i];
/**************FORMAT 1**************/
		if(SicInstCheck(curbyte)==1)
		{
			printf("Format 1: %6s ", 
							SicInstMnemonic(curbyte));
			instptr->startadr=locctr;
			instptr->opname[0]=' ';
			instptr = OpcodeCopy(curbyte, instptr);
			BinaryPrint(inst[i]);
			locctr++;
			instptr->format=1;
			instptr->objcode[0] = inst[i];
		}
/**************FORMAT 2**************/
		else if(SicInstCheck(curbyte)==2)
		{
			printf("Format 2: %6s ", SicInstMnemonic(curbyte));
			instptr->startadr=locctr;
			instptr->opname[0]=' ';
			instptr = OpcodeCopy(curbyte, instptr);
			unsigned char r1 = ByteToHalfByte(inst[i+1], 1);
			unsigned char r2 = ByteToHalfByte(inst[i+1], 0);
			BinaryPrint(inst[i]);
			BinaryPrint(inst[i+1]);
			printf("				r1 %01X,	r2 %01X", r1, r2);
			locctr+=2;
			instptr->format=2;
			instptr->objcode[0] = inst[i];
			instptr->objcode[1] = inst[i+1];
			i++;
		}
/****************FORMAT 0/3/4*****************/
		else
		{
			/*check right-most bits of first 
			byte so see if format 0*/
/*****************FORMAT 0******************/
			if( Bit(curbyte,1)==0 && Bit(curbyte,0)==0 )
			{
				printf("Format 0: %6s ", SicInstMnemonic(curbyte));
				instptr->startadr=locctr;
				instptr->opname[0]=' ';
				instptr = OpcodeCopy(curbyte, instptr);
				unsigned int disp1 = (Bit(inst[i+1],6)<<14) + (Bit(inst[i+1],5)<<13) + (Bit(inst[i+1],4)<<12);
				unsigned int disp2 = ByteToHalfByte(inst[i+1], 0) << 8;
				unsigned char disp3 = inst[i+2];
				enum boolean xflag=false;
				if(Bit(inst[i+1],7)==1)
					xflag=true;
				BinaryPrint(inst[i]);
				BinaryPrint(inst[i+1]);
				BinaryPrint(inst[i+2]);
				unsigned int disp = disp1 + disp2 + disp3;
				printf("			x %1d	disp	%03X", xflag, disp);
				locctr+=3;
				instptr->format=3;
				instptr->objcode[0] = inst[i];
				instptr->objcode[1] = inst[i+1];
				instptr->objcode[2] = inst[i+2];
				i+=2;
			}
			//have a method to grab 2nd byte to check if format 3 or 4
/******************FORMAT 3********************/
			else
			{
				if(Bit(inst[i+1],4)==0)
				{
					unsigned int targetaddress;
					printf("Format 3: %6s ", SicInstMnemonic(curbyte));
					instptr->startadr=locctr;
					instptr->opname[0]=' ';
					instptr = OpcodeCopy(curbyte, instptr);
					int disp1 = ByteToHalfByte(inst[i+1], 0) << 8;
					char disp2 = inst[i+2];
					int disp = disp1 + disp2;
					enum boolean xflag=false;
					char* addressingmode;
					if(Bit(inst[i+1],7)==1)
						xflag=true;
					if(Bit(curbyte,1)==1&&Bit(curbyte,0)==0)
					{
						addressingmode = "Indirect";
						instptr->operand[0]='@';
					}
					else if(Bit(curbyte,1)==0&&Bit(curbyte,0)==1)
					{
						addressingmode = "Immediate";
						instptr->operand[0]='#';
					}
					else
					{
						instptr->operand[0]=' ';
						if(Bit(inst[i+1],5)==1)
						{
							addressingmode = "PC Relative";
							targetaddress = disp + locctr + 3;
							if(baseflag==true)
							{
								Instruction* instptrbase = malloc(sizeof(Instruction));
								instptrbase->startadr=locctr;
								instptrbase->opname[0] = ' ';
								instptrbase->opname[1] = 'N';
								instptrbase->opname[2] = 'O';
								instptrbase->opname[3] = 'B';
								instptrbase->opname[4] = 'A';
								instptrbase->opname[5] = 'S';
								instptrbase->opname[6] = 'E';
								instptrbase->opname[7] = '\0';
								head = Add(head, instptrbase);
								baseflag=false;
							}
						}
						else if(Bit(inst[i+1],6)==1)
						{
							addressingmode = "Base Relative";
							if(baseflag==false)
							{
								Instruction* instptrbase = malloc(sizeof(Instruction));
								instptrbase->startadr=locctr;
								instptrbase->opname[0] = ' ';
								instptrbase->opname[1] = 'B';
								instptrbase->opname[2] = 'A';
								instptrbase->opname[3] = 'S';
								instptrbase->opname[4] = 'E';
								instptrbase->opname[5] = '\0';
								head = Add(head, instptrbase);
								baseflag=true;
							}
						}
						else
						{
							addressingmode = "Direct";
							targetaddress = disp;
						}
					}
					BinaryPrint(inst[i]);
					BinaryPrint(inst[i+1]);
					BinaryPrint(inst[i+2]);
					printf("			x %1d	TA  	%04X   %s", xflag, targetaddress, addressingmode);
					locctr+=3;
					instptr->format=3;
					instptr->objcode[0] = inst[i];
					instptr->objcode[1] = inst[i+1];
					instptr->objcode[2] = inst[i+2];
					i+=2;
				}
/*****************FORMAT 4********************/
				else
				{
					printf("Format 4: %6s ", SicInstMnemonic(curbyte));
					instptr->startadr=locctr;
					instptr->opname[0]='+';
					instptr = OpcodeCopy(curbyte, instptr);
					unsigned int address1 = ByteToHalfByte(inst[i+1], 0) << 16;
					unsigned int address2 = inst[i+2] << 8;
					unsigned char address3 = inst[i+3];
					enum boolean xflag=false;
					char* addressingmode;
					if(Bit(inst[i+1],7)==1)
						xflag=true;
					if(Bit(curbyte,1)==1&&Bit(curbyte,0)==0)
					{
						addressingmode = "Indirect";
						instptr->operand[0]='@';
					}
					else if(Bit(curbyte,1)==0&&Bit(curbyte,0)==1)
					{
						addressingmode = "Immediate";
						instptr->operand[0]='#';
					}
					else
					{
						instptr->operand[0]=' ';
						addressingmode = "Direct";
					}
					BinaryPrint(inst[i]);
					BinaryPrint(inst[i+1]);
					BinaryPrint(inst[i+2]);
					BinaryPrint(inst[i+3]);
					unsigned int address = address1 + address2 + address3;
					printf("	x %1d	adrs	%05X  %s", xflag, address, addressingmode);
					locctr+=4;
					instptr->format=4;
					instptr->objcode[0] = inst[i];
					instptr->objcode[1] = inst[i+1];
					instptr->objcode[2] = inst[i+2];
					instptr->objcode[3] = inst[i+3];
					i+=3;
				}
			}
		}
		printf("\n");
		head = Add(head, instptr);
    }
	return head;
}

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

/*************************************************************
 function: OpcodeCopy
 Notes: Puts opcode into Instruction struct.
 I/O: input paramaters: A byte of data
      output: 
 *************************************************************/

Instruction* OpcodeCopy(unsigned char curbyte, 
									Instruction* instptr)
{
	char* opcode = SicInstMnemonic(curbyte);
	int i;
    for(i=1;i<strlen(opcode)+1;i++)
	{
		instptr->opname[i] = opcode[i-1];
    }
	while(i!=7)
	{
		instptr->opname[i] = ' ';
		i++;
	}
	instptr->opname[7]='\0';
	return instptr;
}
/**********************[ EOF: format.c ]***********************/
