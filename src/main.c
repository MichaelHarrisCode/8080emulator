#include "../include/8080emu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

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
	system("clear");

	// Currently, our program should always take 3 total arguments.
	// This makes sure it's 3.
	if (argc < 3) {
		printf("\033[0;31merror\033[0m: Missing arguments\n");
		exit(EXIT_FAILURE);
	} else if (argc > 4) {
		printf("\033[0;31merror\033[0m: Too many arguments\n");
		exit(EXIT_FAILURE);
	}

	// The third argument should always be a file.
	FILE* file = fopen(argv[2], "rb");

	// Makes sure the file exists.
	if (file == NULL) {
	    printf("\033[0;31merror\033[0m: File couldn't be loaded\n");
	    exit(EXIT_FAILURE);
	}


	// Runs code based on command line arguments
	// Disassembling
	if (strncmp(argv[1], "das", 3) == 0) {
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
	// Running
	} else if (strncmp(argv[1], "run", 3) == 0) {
		State8080 emu;
		init_8080(&emu, file);

		for (;;) {
			emulate8080Op(&emu);
		}
	} else if (strncmp(argv[1], "dbg", 3) == 0) {
		State8080 emu;
		init_8080(&emu, file);

		for (;;) {
			emulate8080Op(&emu);
			getchar();
		}
	} else {
		printf("\033[0;31merror\033[0m: Unrecognized Command\n");
	}

	return 0;
}