/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: optab.c
 Notes: There will probably be methods associated with
    printing in here.
    TBA
 *************************************************************/
#include <stdlib.h>
#include "optab.h"

int sicInstCheck(unsigned char curByte){
	unsigned char opCode=curByte>>2;
	opCode<<=2;
	switch(opCode)
	{
		//Format 1 Check
		case 0xC4:
			return 1;
		case 0xC0:
			return 1;
		case 0xF4:
			return 1;
		case 0xC8:
			return 1;
		case 0xF0:
			return 1;
		case 0xF8:
			return 1;
			
		//Format 2 Check
		case 0x90:
			return 2;
		case 0xB4:
			return 2;
		case 0xA0:
			return 2;
		case 0x9C:
			return 2;
		case 0x98:
			return 2;
		case 0xAC:
			return 2;
		case 0xA4:
			return 2;
		case 0xA8:
			return 2;
		case 0x94:
			return 2;
		case 0xB0:
			return 2;
		case 0xB8:
			return 2;
			
		//Format 0/3/4 Check
		default:
			return 0;
	}
}

char* sicInstMnemonic(unsigned char curByte){
	unsigned char opCode=curByte>>2;
	opCode<<=2;
	switch(opCode)
	{
		//Format 1 Check
		case 0xC4:
			return "FIX";
		case 0xC0:
			return "FLOAT";
		case 0xF4:
			return "HIO";
		case 0xC8:
			return "NORM";
		case 0xF0:
			return "SIO";
		case 0xF8:
			return "TIO";
			
		//Format 2 Check
		case 0x90:
			return "ADDR";
		case 0xB4:
			return "CLEAR";
		case 0xA0:
			return "COMPR";
		case 0x9C:
			return "DIVR";
		case 0x98:
			return "MULR";
		case 0xAC:
			return "RMO";
		case 0xA4:
			return "SHIFTL";
		case 0xA8:
			return "SHIFTR";
		case 0x94:
			return "SUBR";
		case 0xB0:
			return "SVC";
		case 0xB8:
			return "TIXR";
			
		//Format 0/3/4 Check
		case 0x18:
			return "ADD";
		case 0x58:
			return "ADDF";
		case 0x40:
			return "AND";
		case 0x28:
			return "COMP";
		case 0x88:
			return "COMPF";
		case 0x24:
			return "DIV";
		case 0x64:
			return "DIVF";
		case 0x3C:
			return "J";
		case 0x30:
			return "JEQ";
		case 0x34:
			return "JGT";
		case 0x38:
			return "JLT";
		case 0x48:
			return "JSUB";
		case 0x00:
			return "LDA";
		case 0x68:
			return "LDB";
		case 0x50:
			return "LDCH";
		case 0x70:
			return "LDF";
		case 0x08:
			return "LDL";
		case 0x6C:
			return "LDS";
		case 0x74:
			return "LDT";
		case 0x04:
			return "LDX";
		case 0xD0:
			return "LPS";
		case 0x20:
			return "MUL";
		case 0x60:
			return "MULF";
		case 0x44:
			return "OR";
		case 0xD8:
			return "RD";
		case 0x4C:
			return "RSUB";
		case 0xEC:
			return "SSK";
		case 0x0C:
			return "STA";
		case 0x78:
			return "STB";
		case 0x54:
			return "STCH";
		case 0x80:
			return "STF";
		case 0xD4:
			return "STI";
		case 0x14:
			return "STL";
		case 0x7C:
			return "STS";
		case 0xE8:
			return "STSW";
		case 0x84:
			return "STT";
		case 0x10:
			return "STX";
		case 0x1C:
			return "SUB";
		case 0x5C:
			return "SUBF";
		case 0xE0:
			return "TD";
		case 0x2C:
			return "TIX";
		case 0xDC:
			return "WD";
		default:
			printf("No such opcode.\n");
			exit(1);
	}
}

/*******************[ EOF: optab.c ]********************/