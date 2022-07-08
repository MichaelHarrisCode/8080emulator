#include "../include/8080emu.h"
#include <stdio.h>
#include <stdlib.h>

State8080 *new_8080()
{
	State8080 *temp = malloc(sizeof(State8080));

	// Initalize PSW condition bits
	temp->cc.BIT_1 = 1;
	temp->cc.BIT_3 = 0;
	temp->cc.BIT_5 = 0;

	return temp;
}

void free_8080(State8080 *self)
{
	free(self);
}

// Will run if an unimplemented instruction is called.
void unimplementedInstruction(uint8_t code)
{
	printf("error: Missing Code Definition for 0x%02x\n", code);
	exit(EXIT_FAILURE);
}

// Concatenates an address from high and low bits
uint16_t address_concat(uint8_t low, uint8_t high)
{
	return high << 8 | low;
}





/**
 * 
 * Flag related functions
 * 
 */

// Calculates Parity
int parity(uint8_t value)
{
	int counter = 0;
	for (int i = 0; i < 8; i++)
	{	
		if (value & 0x1)
			counter++;
		
		value >>= 1;
	}
	
	return (counter % 2 == 0);
}

// Sets flags Z, S, and P
void flagsZSP(State8080 *self, uint8_t value)
{
	// Zero flag
	self->cc.z = (value == 0);

	// Sign flag
	self->cc.s = (0x80 == (value & 0x80));

	// Parity flag
	self->cc.p = parity(value);
}

// Sets flag AC
// I may have to do this flag BEFORE performing the operation
void flagAC(State8080 *self, uint8_t initial, uint16_t added)
{
	initial = (initial & 0xf);
	added = (added & 0xf);

	self->cc.ac = ((uint8_t)(initial + added) > 0xf);
}