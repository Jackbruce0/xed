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
#include "symbol.h"
#include "xedlib.h"

enum boolean {false, true};

/*************************************************************
 function: FormatCall
 Notes: Fills Instruction struct linked list
 I/O: input paramaters: reclength of Text Record,inst
						array holding contents, and head.
      output: Returns Head of linked list
 *************************************************************/

link FormatCall(unsigned int reclength, 
				unsigned char inst[30], link head,
				unsigned int locctr)
{
    unsigned char curbyte;
    int i, basevalue = 0;
	enum boolean baseflag = false;
	
    for(i=0; i<reclength; i++)
	{
		Instruction* instptr = malloc(sizeof(Instruction));
        
        //Initialize your variables jake!
        instptr->startadr = 0;
        strncpy(instptr->operand, "         \0", 10);
        
        
		curbyte = inst[i];
        /* Symbol Check */
		strncpy(instptr->label, GetSymbolName(locctr), 7);
        /* Literal Check */
        Literal *lit = GetLiteral(locctr);
        if (lit != NULL)
        {
            /* an LTORG statement prior (if not EOF) */
            instptr = InsertLiteral(lit, inst, locctr, i);
            locctr += instptr->format;
            Add(head,instptr);
            continue;
        }
        /* End of Literal Check */
/**************FORMAT 1**************/
		if(SicInstCheck(curbyte)==1)
		{
			//Debugging Prints
			printf("Format 1: %6s ", 
					SicInstMnemonic(curbyte));
			BinaryPrint(inst[i]);
			//
			instptr = Format1(instptr, curbyte,
						locctr, i, inst);
			locctr++;
		}
/**************FORMAT 2**************/
		else if(SicInstCheck(curbyte)==2)
		{
			//Debugging Prints
			printf("Format 2: %6s ",
					SicInstMnemonic(curbyte));
			BinaryPrint(inst[i]);
			BinaryPrint(inst[i+1]);
			//
			instptr = Format2(instptr, curbyte,
						locctr, i, inst);
			locctr+=2;
			i++;
		}
/****************FORMAT 0*****************/
		else if( Bit(curbyte,1)==0 && Bit(curbyte,0)==0 )
		{
			//Debugging Prints
			printf("Format 0: %6s ",
					SicInstMnemonic(curbyte));
			BinaryPrint(inst[i]);
			BinaryPrint(inst[i+1]);
			BinaryPrint(inst[i+2]);
			//
			instptr = Format0(instptr, curbyte,
						locctr, i, inst);
			locctr+=3;
			i+=2;
		}
/******************FORMAT 3********************/
		else if(Bit(inst[i+1],4)==0)
		{
			//Debugging Prints
			printf("Format 3: %6s ",
					SicInstMnemonic(curbyte));
			BinaryPrint(inst[i]);
			BinaryPrint(inst[i+1]);
			BinaryPrint(inst[i+2]);
			//
			if(Bit(inst[i+1],5)==1)/*Base Directive Check*/
			{
				/*
				if(baseflag==true)
				{
					Instruction* instptrbase = malloc(sizeof(Instruction));
					instptrbase->format = -1;
					instptrbase->startadr=locctr;
					istrncpy(instptrbase->label, "      \0", 7);
					strncpy(instptrbase->opname, " NOBASE\0",9);
					strncpy(instptrbase->operand, "      \0", 7);
					head = Add(head, instptrbase);
					baseflag=false;
				}
				*/
			}
			else if(Bit(inst[i+1],6)==1)
			{
				if(baseflag==false)
				{
					basevalue = locctr;
					Instruction* instptrbase = malloc(sizeof(Instruction));
					instptrbase->format = -1;
					instptrbase->startadr=locctr;
					strncpy(instptrbase->label, "      \0", 7);
					strncpy(instptrbase->opname, " BASE \0",8);
					strncpy(instptrbase->operand, "      \0", 7);
					head = Add(head, instptrbase);
					baseflag=true;
				}
			}
			instptr = Format3(instptr, curbyte,
						locctr, i, inst, basevalue);
			locctr+=3;
			i+=2;
		}
/*****************FORMAT 4********************/
		else
		{
			//Debugging Prints
			printf("Format 4: %6s ",
					SicInstMnemonic(curbyte));
			BinaryPrint(inst[i]);
			BinaryPrint(inst[i+1]);
			BinaryPrint(inst[i+2]);
			BinaryPrint(inst[i+3]);
			//
			instptr = Format4(instptr, curbyte,
						locctr, i, inst);
			locctr+=4;
			i+=3;
		}
		printf("\n");
		head = Add(head, instptr);
    }
	return head;
}

/*************************************************************
 function: Format1
 Notes: Fills instptr with Format 1 instruction
 I/O: input paramaters: Current instptr, current byte of the
		data, the locctr, the i index of the text record data,
		and the text record data.
      output: Returns the filled instptr
 *************************************************************/

Instruction* Format1(Instruction* instptr,
				unsigned char curbyte, unsigned int locctr,
				int i, unsigned char inst[30])
{
	instptr->startadr=locctr;
	instptr->opname[0]=' ';
	instptr = OpcodeCopy(curbyte, instptr);
	instptr->format=1;
	instptr->objcode[0] = inst[i];
	return instptr;
}

/*************************************************************
 function: Format2
 Notes: Fills instptr with Format 2 instruction
 I/O: input paramaters: Current instptr, current byte of the
		data, the locctr, the i index of the text record data,
		and the text record data.
      output: Returns the filled instptr 
 *************************************************************/

Instruction* Format2(Instruction* instptr,
				unsigned char curbyte, unsigned int locctr,
				int i, unsigned char inst[30])
{
	instptr->startadr=locctr;
	instptr->opname[0]=' ';
	instptr = OpcodeCopy(curbyte, instptr);
	unsigned char r1 = ByteToHalfByte(inst[i+1], 1);
	unsigned char r2 = ByteToHalfByte(inst[i+1], 0);
	strncpy(instptr->operand+1, RegisterTab(r1), 2);
	if(NumOfRegisters(curbyte)==1)//check if instruction needs 2nd register
	{
		strncpy(instptr->operand+1+strlen(RegisterTab(r1)), ",", 1);
		strncpy(instptr->operand+2+strlen(RegisterTab(r1)), RegisterTab(r2), 2);
	}
	int j;
	for(j=2; j<8; j++)/*Fills in extra space*/
	{
		if(instptr->operand[j]<44)
			instptr->operand[j]=' ';
	}
	printf("				r1 %01X,	r2 %01X", r1, r2);//Debugger
	instptr->format=2;
	instptr->objcode[0] = inst[i];
	instptr->objcode[1] = inst[i+1];
	return instptr;
}

/*************************************************************
 function: Format0
 Notes: Fills instptr with Format 0 instruction
  I/O: input paramaters: Current instptr, current byte of the
		data, the locctr, the i index of the text record data,
		and the text record data.
      output: Returns the filled instptr
 *************************************************************/

Instruction* Format0(Instruction* instptr,
				unsigned char curbyte, unsigned int locctr,
				int i, unsigned char inst[30])
{
	instptr->startadr=locctr;
	instptr->opname[0]=' ';
	instptr = OpcodeCopy(curbyte, instptr);
	unsigned int disp1 = (Bit(inst[i+1],6)<<14) + (Bit(inst[i+1],5)<<13) + (Bit(inst[i+1],4)<<12);
	unsigned int disp2 = ByteToHalfByte(inst[i+1], 0) << 8;
	unsigned char disp3 = inst[i+2];
	enum boolean xflag=false;
	if(Bit(inst[i+1],7)==1)
		xflag=true;
	unsigned int targetaddress = disp1 + disp2 + disp3;
	strncpy(instptr->operand+1, GetSymbolName(targetaddress), 7);
	if(xflag==true)
	{
		strncpy(instptr->operand+(strlen(GetSymbolName(targetaddress))+1), ",X", 2);
	}
	else
	{
		strncpy(instptr->operand+(strlen(GetSymbolName(targetaddress))+1), "  ", 2);
	}
	printf("			x %1d	target	%05X", xflag, targetaddress);//Debugger
	instptr->format=3;
	instptr->objcode[0] = inst[i];
	instptr->objcode[1] = inst[i+1];
	instptr->objcode[2] = inst[i+2];
	return instptr;
}

/*************************************************************
 function: Format3
 Notes: Fills instptr with Format 3 instruction
  I/O: input paramaters: Current instptr, current byte of the
		data, the locctr, the i index of the text record data,
		the text record data, and the saved basevalue.
      output: Returns the filled instptr
 *************************************************************/

Instruction* Format3(Instruction* instptr,
				unsigned char curbyte, unsigned int locctr,
				int i, unsigned char inst[30], int basevalue)
{
	unsigned int targetaddress;
	instptr->startadr=locctr;
	instptr->opname[0]=' ';
	instptr = OpcodeCopy(curbyte, instptr);
	int disp1 = ByteToHalfByte(inst[i+1], 0) << 8;
	int disp2 = inst[i+2];
	int disp = disp1 + disp2;
	if(Bit(ByteToHalfByte(inst[i+1],0),3)==1)
	{
		disp=-(0x1000-disp);/*Makes disp negative*/
	}
	enum boolean xflag=false;
	char* addressingmode;
	if(Bit(inst[i+1],7)==1)/*Indexed addressing check*/
		xflag=true;
		
	/*Indirect or Immediate check*/
	if(Bit(curbyte,1)==1&&Bit(curbyte,0)==0)
	{
		//addressingmode = "Indirect";
		instptr->operand[0]='@';
	}
	else if(Bit(curbyte,1)==0&&Bit(curbyte,0)==1)
	{
		//addressingmode = "Immediate";
		instptr->operand[0]='#';
	}
	else
	{
		instptr->operand[0]=' ';
	}
	
	if(Bit(inst[i+1],5)==1)/*Addressing mode check*/
	{
		addressingmode = "PC Relative";
		targetaddress = disp + locctr + 3;
	}
	else if(Bit(inst[i+1],6)==1)
	{
		addressingmode = "Base Relative";
		targetaddress = disp + basevalue;
	}
	else
	{
		addressingmode = "Direct";
		targetaddress = disp;
	}
	
	strncpy(instptr->operand+1, GetSymbolName(targetaddress), 7);
	if(xflag==true)
	{
		strncpy(instptr->operand+(strlen(GetSymbolName(targetaddress))+1), ",X", 2);
	}
	else
	{
		strncpy(instptr->operand+(strlen(GetSymbolName(targetaddress))+1), "  ", 2);
	}
	instptr->format=3;
	instptr->objcode[0] = inst[i];
	instptr->objcode[1] = inst[i+1];
	instptr->objcode[2] = inst[i+2];
	printf("			x %1d	TA  	%06X   %s", xflag, targetaddress, addressingmode);//Debugger
	return instptr;
}

/*************************************************************
 function: Format4
 Notes: Fills instptr with Format 4 instruction
  I/O: input paramaters: Current instptr, current byte of the
		data, the locctr, the i index of the text record data,
		and the text record data.
      output: Returns the filled instptr
 *************************************************************/

Instruction* Format4(Instruction* instptr,
				unsigned char curbyte, unsigned int locctr,
				int i, unsigned char inst[30])
{
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
	unsigned int address = address1 + address2 + address3;
	strncpy(instptr->operand+1, GetSymbolName(address), 7);
	if(xflag==true)
	{
		strncpy(instptr->operand+(strlen(GetSymbolName(address))+1), ",X", 2);
	}
	else
	{
		strncpy(instptr->operand+(strlen(GetSymbolName(address))+1), "  ", 2);
	}
	instptr->format=4;
	instptr->objcode[0] = inst[i];
	instptr->objcode[1] = inst[i+1];
	instptr->objcode[2] = inst[i+2];
	instptr->objcode[3] = inst[i+3];
	printf("	x %1d	adrs	%05X  %s", xflag, address, addressingmode);//Debugger
	return instptr;
}

/*************************************************************
 function: InsertRESDirectives
 Notes: Compare current address with address of next
    instruction or symbol to determine if a RESB or RESW
    directive should inserted
 I/O: input paramaters: LOCCTR and nextaddr (Address of next
        instruction)
      output: void
 *************************************************************/
link InsertRESDirectives(link HEAD, int LOCCTR, int nextaddr)
{
    while(LOCCTR < nextaddr)
    {
        int size = 0, nextsymaddr = 0;
        Instruction *RES = malloc(sizeof(Instruction));
        RES->format = -1;
        RES->objcode[0] = ' ';
        RES->objcode[1] = ' ';
        RES->objcode[2] = ' ';
        RES->objcode[3] = '\0';
        RES->startadr = LOCCTR;
        strncpy(RES->label, GetSymbolName(LOCCTR), 7);
        nextsymaddr = NextSymbolAddress(LOCCTR);
        if(nextaddr > nextsymaddr)
        {
            size = nextsymaddr - LOCCTR;
            LOCCTR = nextsymaddr;
        } else
        {
            size = nextaddr - LOCCTR;
            LOCCTR = nextaddr;
        }
        if(size % 3 == 0)
        {
            strncpy(RES->opname, " RESW  \0", 8);
            sprintf(RES->operand, " %d", size/3);
        }else
        {
            strncpy(RES->opname, " RESB  \0", 8);
            sprintf(RES->operand, " %d", size);
        }
        HEAD = Add(HEAD, RES);
    }
    return HEAD;
} /* End of function Insert_RES_Directives */

/*************************************************************
 function: InsertLiteral
 Notes:
 I/O: input paramaters:
      output: void
 *************************************************************/
Instruction *InsertLiteral(Literal *lit,
                           unsigned char inst[30], int locctr ,
                           int i)
{
    /* an LTORG statement prior (if not EOF) */
    Instruction *instptr = malloc(sizeof(Instruction));
    instptr->startadr = locctr;
    strncpy(instptr->label, "*     \0", 7);
    strncpy(instptr->opname, lit->literal, 8);
    int bytesize = lit->length / 2; /* length is
                                     in half bytes */
    instptr->format = bytesize; /* for printing */
    int q = 0;
    while(q < bytesize)
    {
        instptr->objcode[q] = inst[i + q];
        q++;
    }
    return instptr;
} /* End of function Insert_Literal */

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
