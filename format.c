/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 RedID's: 822320220 & 822843795
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

unsigned int basevalue = 0; /* basevalue is kept track of
							   throughout all of the 
							   Text records. */
 
link FormatCall(unsigned int reclength, 
				unsigned char inst[30], link head,
				unsigned int locctr, int lastbyte)
{
    unsigned char curbyte;
    int i;
    enum boolean needsLTORG = true;
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
            if(needsLTORG && locctr + lit->length/2 < lastbyte)
            {
                head = InsertLTORGDirective(head, locctr);
                needsLTORG = false;
            }
            instptr = InsertLiteral(lit, inst, locctr, i, lastbyte);
            locctr += instptr->format;
            Add(head,instptr);
            continue;
        }
        /* End of Literal Check */
        
/**************FORMAT 1**************/
		if(SicInstCheck(curbyte)==1)
		{
			instptr = Format1(instptr, curbyte,
						locctr, i, inst);
			locctr++;
		}
/*************BYTE of data***************/
		else if(i+1>=reclength)
		{
			instptr = ByteData(instptr, curbyte,
						locctr, i, inst);
			locctr++;
		}
/**************FORMAT 2**************/
		else if(SicInstCheck(curbyte)==2)
		{
			instptr = Format2(instptr, curbyte,
						locctr, i, inst);
			locctr+=2;
			i++;
		}
/****************FORMAT 0*****************/
		else if( Bit(curbyte,1)==0 && Bit(curbyte,0)==0 )
		{
			instptr = Format0(instptr, curbyte,
						locctr, i, inst);
			locctr+=3;
			i+=2;
		}
/******************FORMAT 3********************/
		else if(Bit(inst[i+1],4)==0)
		{
			instptr = Format3(instptr, curbyte,
						locctr, i, inst, basevalue);
			locctr+=3;
			i+=2;
		}
/*****************FORMAT 4********************/
		else
		{
			instptr = Format4(instptr, curbyte,
						locctr, i, inst);
			locctr+=4;
			i+=3;
		}
		head = Add(head, instptr);
		if(strcmp(SicInstMnemonic(curbyte),"LDB")==0)
		{
			int disp1 = ByteToHalfByte(instptr->objcode[1], 0) << 8;
			int disp2 = instptr->objcode[2];
			int disp = disp1 + disp2;
			unsigned int targetaddress = disp + locctr;
			head = InsertBASEDirective(head, 
						locctr-3, targetaddress);
			basevalue = targetaddress;
		}
    }
    needsLTORG = true;
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
	if(NumOfRegisters(curbyte)==1)//2nd register check
	{
		strncpy(instptr->operand+1+strlen(RegisterTab(r1)),
		",", 1);
		strncpy(instptr->operand+2+strlen(RegisterTab(r1)),
		RegisterTab(r2), 2);
	}
	int j;
	for(j=2; j<8; j++)/*Fills in extra space*/
	{
		if(instptr->operand[j]<44)
			instptr->operand[j]=' ';
	}
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
	unsigned int disp1 = (Bit(inst[i+1],6)<<14) +
		(Bit(inst[i+1],5)<<13) + (Bit(inst[i+1],4)<<12);
	unsigned int disp2 = ByteToHalfByte(inst[i+1], 0) << 8;
	unsigned char disp3 = inst[i+2];
	enum boolean xflag=false;
	if(Bit(inst[i+1],7)==1)
		xflag=true;
	unsigned int targetaddress = disp1 + disp2 + disp3;
	strncpy(instptr->operand+1, 
			GetSymbolName(targetaddress), 7);
	if(xflag==true)
	{
		strncpy(instptr->operand+
		(strlen(GetSymbolName(targetaddress))+1), ",X", 2);
	}
	else
	{
		strncpy(instptr->operand+
		(strlen(GetSymbolName(targetaddress))+1), "  ", 2);
	}
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
		sprintf(instptr->operand+1, "%d    ", disp);
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
	
	if(strcmp(GetSymbolName(targetaddress), "      \0")!=0)
	{
		strncpy(instptr->operand+1,
			GetSymbolName(targetaddress), 7);
	}
	
	if(xflag==true)
	{
		strncpy(instptr->operand+
		(strlen(GetSymbolName(targetaddress))+1), ",X", 2);
	}
	else
	{
		strncpy(instptr->operand+
		(strlen(GetSymbolName(targetaddress))+1), "  ", 2);
	}
	instptr->format=3;
	instptr->objcode[0] = inst[i];
	instptr->objcode[1] = inst[i+1];
	instptr->objcode[2] = inst[i+2];
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
	unsigned int address = address1 + address2 + address3;
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
		sprintf(instptr->operand+1, "%d    ", address);
	}
	else
	{
		instptr->operand[0]=' ';
		addressingmode = "Direct";
	}
	
	if(strcmp(GetSymbolName(address), "      \0")!=0)
	{
		strncpy(instptr->operand+1,
			GetSymbolName(address), 7);
	}
	/*Indexed Addressing check*/
	if(xflag==true)
	{
		strncpy(instptr->operand+
		(strlen(GetSymbolName(address))+1), ",X", 2);
	}
	else
	{
		strncpy(instptr->operand+
		(strlen(GetSymbolName(address))+1), "  ", 2);
	}
	instptr->format=4;
	instptr->objcode[0] = inst[i];
	instptr->objcode[1] = inst[i+1];
	instptr->objcode[2] = inst[i+2];
	instptr->objcode[3] = inst[i+3];
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
        if(nextaddr > nextsymaddr && nextsymaddr != -1)
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
                           int i, int lastbyte)
{
    /* an LTORG statement prior (if not EOF) */
    Instruction *instptr = malloc(sizeof(Instruction));
    instptr->startadr = locctr;
    strncpy(instptr->label, "*     \0", 7);
    strncpy(instptr->opname, lit->literal, 8);
    strncpy(instptr->operand, "      \0", 7);
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
 function: ByteData
 Notes: Fills Instruction struct with BYTE data.
 I/O: input paramaters: Current instptr, current byte of the
		data, the locctr, the i index of the text record data,
		and the text record data.
      output: Returns the filled instptr 
 *************************************************************/
Instruction* ByteData(Instruction* instptr,
				unsigned char curbyte, unsigned int locctr,
				int i, unsigned char inst[30])
{
	instptr->startadr=locctr;
	instptr->format = 1;//indicates number of objcode bytes to produce
	strncpy(instptr->opname, "BYTE  ",6);
	instptr->objcode[0] = inst[i];
	instptr->operand[0]=' ';
	strncpy(instptr->operand+1, "X'",2);
	sprintf(instptr->operand+3, "%02d", curbyte);
	strncpy(instptr->operand+5, "'    ",5);
	return instptr;
}

/*************************************************************
 function: InsertSTARTDirective
 Notes: Puts the START directive at the beginning of the
		source code.
 I/O: input paramaters: Head of linked list, program name,
						and the starting address.
      output: void
 *************************************************************/
link InsertSTARTDirective(link HEAD, char* progname, int LOCCTR)
{
    Instruction *START = malloc(sizeof(Instruction));
    START->format = -1;
    START->objcode[0] = ' ';
    START->objcode[1] = ' ';
	START->objcode[2] = ' ';
	START->objcode[3] = '\0';
    START->startadr = LOCCTR;
	strncpy(START->opname, " START \0", 8);
	sprintf(START->operand, " %X", LOCCTR);
    strncpy(START->label, progname, 7);
    HEAD = Add(HEAD, START);
    return HEAD;
} /* End of function Insert_START_Directive */

/*************************************************************
 function: InsertENDDirective
 Notes: Puts the END directive at the end of the
		source code.
 I/O: input paramaters: Head of linked list & starting
						address.
      output: void
 *************************************************************/
link InsertENDDirective(link HEAD, int LOCCTR)
{
    Instruction *END = malloc(sizeof(Instruction));
    END->format = -2;
    END->objcode[0] = ' ';
    END->objcode[1] = ' ';
    END->objcode[2] = ' ';
    END->objcode[3] = '\0';
	strncpy(END->opname, " END   \0", 8);
    strncpy(END->label, "      \0", 7);
	strncpy(END->operand,
			GetSymbolName(LOCCTR), 7);
    HEAD = Add(HEAD, END);
    return HEAD;
} /* End of function Insert_END_Directive */

/*************************************************************
 function: InsertBASEDirective
 Notes: Puts the BASE directive after LDB instruction.
 I/O: input paramaters: Head of linked list & locctr.
      output: link to new head
 *************************************************************/
link InsertBASEDirective(link HEAD, int LOCCTR,
						unsigned int targetaddress)
{
	Instruction *BASE = malloc(sizeof(Instruction));
    BASE->format = -2;
    BASE->objcode[0] = ' ';
    BASE->objcode[1] = ' ';
    BASE->objcode[2] = ' ';
    BASE->objcode[3] = '\0';
	BASE->startadr=LOCCTR;
	strncpy(BASE->opname, "BASE  ",6);
    strncpy(BASE->label, "      \0", 7);
	strncpy(BASE->operand,
			GetSymbolName(targetaddress), 7);
    HEAD = Add(HEAD, BASE);
    return HEAD;
} /* End of function Insert_BASE_Directive */

/*************************************************************
 function: InsertLTORGDirective
 Notes: Puts the LTORG directive after when literal pool is
    declared
 I/O: input paramaters: Head of linked list & locctr.
      output: link to new head
 *************************************************************/
link InsertLTORGDirective(link HEAD, int LOCCTR)
{
    Instruction *LTORG = malloc(sizeof(Instruction));
    LTORG->format = -2;
    LTORG->objcode[0] = ' ';
    LTORG->objcode[1] = ' ';
    LTORG->objcode[2] = ' ';
    LTORG->objcode[3] = '\0';
    LTORG->startadr=LOCCTR;
    strncpy(LTORG->opname, "LTORG ",6);
    strncpy(LTORG->label, "      \0", 7);
    strncpy(LTORG->operand,
            "      \0", 7);
    HEAD = Add(HEAD, LTORG);
    return HEAD;
} /* End of function Insert_BASE_Directive */

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
