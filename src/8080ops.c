#include "../include/8080emu.h"

void lxi_b_d16(State8080 *state, uint8_t first, uint8_t second)
{
	state->c = first;
	state->b = second;
	state->pc += 2;
}

void stax_b(State8080 *state)
{
	state->memory[state->bc] = state->a;
}

void inx_b(State8080 *state)
{
	state->bc++;
}

void inr_b(State8080 *state)
{
	state->b++;
	flagsZSP(state, state->b);
	flagAC(state, state->b, 1);
}

void dcr_b(State8080 *state)
{
	state->b--;
	flagsZSP(state, state->b);
	flagAC(state, state->b, -1);
}

void mvi_b_d8(State8080 *state, uint8_t d8)
{
	state->b = d8;
	state->pc++;
}

//Not sure if this works right. Probably test the logic.
void rlc(State8080 *state)
{
	uint8_t bit_7 = (0x80 == (state->a & 0x80));
	state->a <<= 1;
	state->a = state->a | bit_7;
	state->cc.cy = bit_7;
}

void dad_b(State8080 *state);
void ldax_b(State8080 *state);
void dcx_b(State8080 *state);
void inr_c(State8080 *state);
void dcr_c(State8080 *state);
void mvi_c_d8(State8080 *state, uint8_t d8);
void rrc(State8080 *state);