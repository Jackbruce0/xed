/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: linkedlist.c
 Notes:
 *************************************************************/

#include <stdlib.h>
#include "xedlib.h"

/*************************************************************
 function: Add
 Notes: Adds an Instruction struct pointer to the linked list
 I/O: input paramaters: head of the list and 
									Instruction pointer
      output: Returns altered head of linked list
 *************************************************************/
link Add(link head, Instruction *inst)
{
	//problem between here
	link listptr = head;
	link newbie = malloc(sizeof(*newbie));
	newbie->instptr = malloc(sizeof(Instruction*));
	newbie->instptr = inst;
	newbie->next=NULL;
	//and here

	if(head==NULL)
		head = newbie;
	else
	{
		while(listptr->next!=NULL)
			listptr = listptr->next;
		listptr->next=newbie;
	}
    return head;
}

/*************************************************************
 function: GetLocctr
 Notes: Gets the starting address for the next Text record
		being sent.
 I/O: input paramaters: Head of the linked list
      output: unsigned int locctr, the last location address
 *************************************************************/
unsigned int GetLocctr(link head)
{
    link listptr = head;
	if(head==NULL)
	{
		return 0;
	}
	while(listptr->next!=NULL)
	{
		listptr = listptr->next;
	}
	if(listptr->instptr->format==1)
		return listptr->instptr->startadr+1;
	else if(listptr->instptr->format==2)
		return listptr->instptr->startadr+2;
	else if(listptr->instptr->format==4)
		return listptr->instptr->startadr+4;
	else
		return listptr->instptr->startadr+3;
} /* End function GetLocctr */

/*************************************************************
 function: PrintList
 Notes: Prints out instruction struct contents. This will be
		the main function for printing out the source code.
 I/O: input paramaters: Head of the linked list.
      output: N/A
 *************************************************************/

void PrintList(link head)
{
	link listptr = head;
	int j;
	while(listptr!=NULL)
	{
		printf("%04X  ", listptr->instptr->startadr);

		printf("%6s ", listptr->instptr->label);

        printf("%7s  ", listptr->instptr->opname);
        
        // operand[0] is unitialized often
        listptr->instptr->operand[9] = '\0';
		printf("%7s   ", listptr->instptr->operand);
        
        
        int format = listptr->instptr->format;
        int objlen = 0;
        if(format == 0 || format == 3)
        {
            objlen = 3;
        } else if (format == 4)
        {
            objlen = 4;
        } else if (format == 2)
        {
            objlen = 2;
        } else if (format == 1)
        {
            objlen = 1;
        }
            
        for(j = 0; j < objlen; j++)
        {
            printf("%02X", listptr->instptr->objcode[j]);
        }
        
		printf("\n");
		listptr=listptr->next;
	}
    
	return;
}

/*************************************************************
 function: PrintSIC
 Notes: Prints out instruction struct contents. This will be
		the main function for printing out the source code.
 I/O: input paramaters: Head of the linked list.
      output: N/A
 *************************************************************/

void PrintSIC(link head, char *fileName)
{
	FILE *fPtr = FileOpen(fileName, "wb");
	link listptr = head;
	int j;
	while(listptr!=NULL)
	{
		fprintf(fPtr, "%04X  ", listptr->instptr->startadr);

		fprintf(fPtr, "%6s ", listptr->instptr->label);

        fprintf(fPtr, "%7s  ", listptr->instptr->opname);
        
        // operand[0] is unitialized often
        listptr->instptr->operand[9] = '\0';
		fprintf(fPtr, "%7s   ", listptr->instptr->operand);
        
        
        int format = listptr->instptr->format;
        int objlen = 0;
        if(format == 0 || format == 3)
        {
            objlen = 3;
        } else if (format == 4)
        {
            objlen = 4;
        } else if (format == 2)
        {
            objlen = 2;
        } else if (format == 1)
        {
            objlen = 1;
        }
            
        for(j = 0; j < objlen; j++)
        {
            fprintf(fPtr, "%02X", listptr->instptr->objcode[j]);
        }
        
		fprintf(fPtr, "\n");
		listptr=listptr->next;
	}
    fclose(fPtr);
	return;
}

/*************************************************************
 function: PrintLisFile
 Notes: Prints out instruction struct contents. This will be
		the main function for printing out the source code.
 I/O: input paramaters: Head of the linked list.
      output: N/A
 *************************************************************/

void PrintLisFile(link head, char *fileName)
{
	FILE *fPtr = FileOpen(fileName, "wb");
	link listptr = head;
	int j;
	while(listptr!=NULL)
	{
		fprintf(fPtr, "%6s ", listptr->instptr->label);

        fprintf(fPtr, "%7s  ", listptr->instptr->opname);
        
        // operand[0] is unitialized often
        listptr->instptr->operand[9] = '\0';
		fprintf(fPtr, "%7s   ", listptr->instptr->operand);
		fprintf(fPtr, "\n");
		listptr=listptr->next;
	}
    fclose(fPtr);
	return;
}

/*************************************************************
 function: FreeItems
 Notes: Frees memory of each Instruction pointer called using
												malloc.
 I/O: input paramaters: Head of linked list
      output: N/A
 *************************************************************/
void FreeItems(link head)
{
	link listPtr = head;
	while(listPtr!=NULL){
		free(listPtr->instptr);
		listPtr=listPtr->next;
	}
	return;
}

/*************************************************************
 function: FreeList
 Notes: Free memory of each node created in LList.
 I/O: input paramaters: Head of linked list
      output: N/A
 *************************************************************/

void FreeList(link head)
{
	link listPtr;
	while(head!=NULL){
		listPtr = head;
		head = head->next;
		free(listPtr);
	}
	return;
}
/*******************[ EOF: instruction.c ]********************/
