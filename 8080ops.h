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
void lxi_d_d16(State8080 *state, uint8_t first, uint8_t second);
void stax_d(State8080 *state);
void inx_d(State8080 *state);
void inr_d(State8080 *state);
void dcr_d(State8080 *state);
void mvi_d_d8(State8080 *state, uint8_t d8);
void ral(State8080 *state);
void dad_d(State8080 *state);
void ldax_d(State8080 *state);
void dcx_d(State8080 *state);
void inr_e(State8080 *state);
void dcr_e(State8080 *state);
void mvi_e_d8(State8080 *state, uint8_t d8);
void rar(State8080 *state);

// 20
void lxi_h_d16(State8080 *state, uint8_t first, uint8_t second);
void shld_adr(State8080 *state, uint8_t first, uint8_t second);
void inx_h(State8080 *state);
void inr_h(State8080 *state);
void dcr_h(State8080 *state);
void mvi_h_d8(State8080 *state, uint8_t d8);
void daa(State8080 *state);
void dad_h(State8080 *state);
void lhld_adr(State8080 *state, uint8_t first, uint8_t second);
void dcx_h(State8080 *state);
void inr_l(State8080 *state);
void dcr_l(State8080 *state);
void mvi_l_d8(State8080 *state, uint8_t d8);
void cma(State8080 *state);

// 30
void lxi_sp_d16(State8080 *state, uint8_t first, uint8_t second);
void sta_adr(State8080 *state, uint8_t first, uint8_t second);
void inx_sp(State8080 *state);
void inr_m(State8080 *state);
void dcr_m(State8080 *state);
void mvi_m_d8(State8080 *state, uint8_t d8);
void stc(State8080 *state);
void dad_sp(State8080 *state);
void lda_adr(State8080 *state, uint8_t first, uint8_t second);
void dcx_sp(State8080 *state);
void inr_a(State8080 *state);
void dcr_a(State8080 *state);
void mvi_a_d8(State8080 *state, uint8_t d8);
void cmc(State8080 *state);

// 40

#endif