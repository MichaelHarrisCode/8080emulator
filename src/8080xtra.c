#include "../include/8080emu.h"
#include <stdio.h>
#include <stdlib.h>

void init_8080(State8080 *self)
{
	self->cc.cy = 0;
	self->cc.p = 0;
	self->cc.ac = 0;
	self->cc.z = 0;
	self->cc.s = 0;

	// Initalize PSW condition bits
	self->cc.BIT_1 = 1;
	self->cc.BIT_3 = 0;
	self->cc.BIT_5 = 0;
	
	self->pc = 0;
	self->sp = 0;

	self->bc = 0;
	self->de = 0;
	self->hl = 0;
	self->a = 0;

	self->pc_inc = 1;
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