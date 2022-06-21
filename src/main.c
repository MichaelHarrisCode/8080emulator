#include "../include/8080emu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 
 * Need to design this to accept arguments
 * I'm thinking we have run and da
 * 
 * run - Runs the file
 * das - Disassembles the file to the screen
 * 
 */

int main(int argc, char **argv)
{
	// Currently, our program should always take 3 total arguments.
	// This makes sure it's 3.
	if (argc < 3) {
		printf("error: Missing arguments\n");
		if (argc == 2) {
			printf("error: Missing filename\n");
		}
		exit(EXIT_FAILURE);
	} else if (argc > 4) {
		printf("error: Too many arguments\n");
		exit(EXIT_FAILURE);
	}

	// The third argument should always be a file.
	FILE* file = fopen(argv[2], "rb");

	// Makes sure the file exists.
	if (file == NULL) {
	    printf("error: File couldn't be loaded\n");
	    exit(EXIT_FAILURE);
	}


	// Runs code based on command line argument
	if (strncmp(argv[1], "das", 3) == 0) {
		// Gets file size
		fseek(file, 0l, SEEK_END);
		int file_size = ftell(file);
		fseek(file, 0l, SEEK_SET);

		uint8_t *buffer = malloc(file_size);

		fread(buffer, file_size, 1, file);
		fclose(file);

		int pc = 0;

		while(pc < file_size)
			pc += disassemble8080Op(buffer, pc);

		free(buffer);
	} else if (strncmp(argv[1], "run", 3) == 0) {
		printf("WIP\n");
	}

	return 0;
}