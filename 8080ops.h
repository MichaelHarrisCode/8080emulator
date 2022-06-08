#ifndef _8080OPS_H
#define _8080OPS_H

#include "8080emu.h"

// 00
void lxi_b_d16(State8080 *state, uint8_t first, uint8_t second);
void stax_b(State8080 *state);
void inx_b(State8080 *state);
void inr_b(State8080 *state);
void dcr_b(State8080 *state);
void mvi_b_d8(State8080 *state, uint8_t d8);
void rlc(State8080 *state);
void dad_b(State8080 *state);
void ldax_b(State8080 *state);
void dcx_b(State8080 *state);
void inr_c(State8080 *state);
void dcr_c(State8080 *state);
void mvi_c_d8(State8080 *state, uint8_t d8);
void rrc(State8080 *state);

// 10

#endif