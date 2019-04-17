/*************************************************************
 Names: Jack Bruce & Jacob Romio
 usernames: cssc0420 & cssc0413
 Project: CS530 Assignment 2
 File: linkedlist.c
 Notes:
 *************************************************************/
#include <stdlib.h>
#include "instruction.h"

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
 function: PrintList
 Notes: Prints out instruction struct contents. This will be
		the main function for printing out the source code.
 I/O: input paramaters: Head of the linked list.
      output: N/A
 *************************************************************/

void PrintList(link head)
{
	link listptr = head;
	int i;
	while(listptr!=NULL)
	{
		for(i=0; i<7; i++)
		{
			printf("%c",listptr->instptr->opname[i]);
		}
		printf("%c", listptr->instptr->operand[0]);
		printf("\n");
		listptr=listptr->next;
	}
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
