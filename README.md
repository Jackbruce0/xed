Names: Jack Bruce and Jacob Romio
Class Accounts: cssc0420(Jack) and cssc0413(Jacob)
Class: CS 530-01 Spring 2019
Assignment #2 SIC XE DISASSEMBLER
File name: README.txt

***[BELOW IS STILL A WORK IN PROGRESS]***
File Manifest
	-main.c
	-main.h
	-README.md
	-Makefile
	-xbd (executable on edoras)

Compile Instructions

	To compile the application, type "make" in the directory with the files to 
	create the executable. To remove *.o files,type "make clean".
	
	
Operating Instructions
	xbd [-b] <filename>
	
	Type "xbd examplefile.xxx" to run the application in standard mode on the 
	indicated file. For binary mode, type "xbd -b exampleFile.xxx".

	
Description of novel design decisions

	Instead of reading the entire file at once, then processing everything, the
	file is read line by line where each line is processed before the next line
	is read. This saves space in memory and prevents us from possibly overloading
	the array data structure in the case of extremely large files.
	
	
Description of any extra functionality not required

	No extra features added.
	
	
Description of all known deficiencies or bugs

	No known bugs, output looks indistinguishable from xxd command.
	
	
Lessons Learned

	-How to process arguments in the command line.
	-Becoming more comfortable with compiling with Makefiles in the command line.
	-Utilizing C for making UNIX commands.