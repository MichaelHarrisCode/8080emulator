#ifndef EMU8080_H
#define EMU8080_H

#include <stdint.h>


typedef struct ConditionCodes ConditionCodes;
typedef struct State8080 State8080;


struct ConditionCodes {
	// Space invaders doesn't use ac
	uint8_t z:1, s:1, p:1, cy:1, ac:1, pad:3;
};

struct State8080 {
	uint8_t a, int_enable, *memory;
	uint16_t sp, pc;
	ConditionCodes cc;

	// B and C registers
	// Union because sometimes combine for BC
	union {
		uint16_t bc;
		struct {
			uint8_t c;
			uint8_t b;
		};
	};

	// D and E registers
	// Union because sometimes combine for DE
	union {
		uint16_t de;
		struct {
			uint8_t e;
			uint8_t d;
		};
	};

	// H and L registers
	// Union because sometimes combine for HL
	union {
		uint16_t hl;
		struct {
			uint8_t l;
			uint8_t h;
		};
	};
};


// Debugging function.
void unimplementedInstruction(uint8_t code);
int parity(uint8_t value);
void flagsZSP(State8080 *state, uint8_t value);
void flagAC(State8080 *state, uint8_t initial, int16_t added);		// CY flag is set in the opcode logic, since it's different by opcode
int disassemble8080Op(uint8_t *codebuffer, int pc);
int emulate8080Op(State8080 *state);

#endif