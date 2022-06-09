#include "8080emu.h"

void lxi_b_d16(State8080 *state, uint8_t first, uint8_t second)
{
	state->c = first;
	state->b = second;
	state->pc += 2;
}