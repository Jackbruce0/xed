Names: Jack Bruce and Jacob Romio
Class Accounts: cssc0420(Jack) and cssc0413(Jacob)
Class: CS 530-01 Spring 2019
Assignment #2 SIC XE DISASSEMBLER
File name: README.txt

For testing, please use .obj/.sym files located in a2/test, and compare output with corresponding
files in a2/test/example

File Manifest:
	-main.c
	-main.h
	-xedlib.c
	-xedlib.h
	-format.c
	-instruction.c
	-instruction.h
	-optab.c
	-optab.h
	-records.c
	-records.h
	-symbol.c
	-symbol.h
	-linkedlist.c
	-README.md
	-Makefile
	-xed (executable on edoras)
Test Files:
	-test/sample.* (test files given) XE
	-test/2_2.* (Figure 2.2) SIC BASIC
	-test/2_6.* (Figure 2.6) XE
	-test/2_10.* (Figure 2.10) XE
	-test/example contains example output of what .sic and .lis files should look like
		-THESE FILES WERE COPIED FROM OUT TEXTBOOK
		-test/example/correct2_2.*
		-test/example/correct2_10.*
		-test/example/sample.* (the .lis file we were given does not match .sic) 

Compile Instructions

	To compile the application, type "make" in the directory with the Makefile (~/a2) to 
	create the executable. To remove *.o files, type "make clean".
	
	
Operating Instructions

	xed <filename>
	
	-file name will not include any extensions
	-filename must contain path to directory that contains <filename>.obj and <filename>.sym
	- For testing, please use .obj/.sym files located in a2/test, and compare output with corresponding
		files in a2/test/example
	ex) xed ./test/2_2 


Description of novel design decisions

	1. Instruction struct was exceptionally useful in linking all phases of this project
		- Instruction struct was designed to completely encapsulate printing a .lis file
		- The complex job of disassembly was simplified to smaller tasks of simply figuring out how to populate 
			the fields of Instruction structs.

	2. Read and tokenize .obj and .sym file into structs prior to processing
		- This greatly simplified the process of interpreting these files
	
	Full list of design decisions are available in our design document

	
Description of any extra functionality not required

	No extra features added.

	
Description of all known deficiencies or bugs
	
	Cannot recognize use of EQU directive.

	BYTE and WORD directives.
		- Works sometimes
	

Lessons Learned

	-Improved knowledge of the C programming language (1st time using this language was with a1).
	-Maintaining organization and good programming practices on a large project like this is very challenging. 
		The need for a detailed design document was made apparent during this project.
	-There are some things that a disassembler can't do with certainty (some of which are mentioned above).
	-Reverse engineering is hard! We felt overwhelmed at certain points of this project.
