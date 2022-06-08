#ifndef _8080OPS_H
#define _8080OPS_H

#include "8080emu.h"

// 0x00
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

// 0x10
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

// 0x20
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

// 0x30
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

// 0x40
void mov_b_b(State8080 *state);
void mov_b_c(State8080 *state);
void mov_b_d(State8080 *state);
void mov_b_e(State8080 *state);
void mov_b_h(State8080 *state);
void mov_b_l(State8080 *state);
void mov_b_m(State8080 *state);
void mov_b_a(State8080 *state);
void mov_c_b(State8080 *state);
void mov_c_c(State8080 *state);
void mov_c_d(State8080 *state);
void mov_c_e(State8080 *state);
void mov_c_h(State8080 *state);
void mov_c_l(State8080 *state);
void mov_c_m(State8080 *state);
void mov_c_a(State8080 *state);

// 0x50
void mov_d_b(State8080 *state);
void mov_d_c(State8080 *state);
void mov_d_d(State8080 *state);
void mov_d_e(State8080 *state);
void mov_d_h(State8080 *state);
void mov_d_l(State8080 *state);
void mov_d_m(State8080 *state);
void mov_d_a(State8080 *state);
void mov_e_b(State8080 *state);
void mov_e_c(State8080 *state);
void mov_e_d(State8080 *state);
void mov_e_e(State8080 *state);
void mov_e_h(State8080 *state);
void mov_e_l(State8080 *state);
void mov_e_m(State8080 *state);
void mov_e_a(State8080 *state);

// 0x60
void mov_h_b(State8080 *state);
void mov_h_c(State8080 *state);
void mov_h_d(State8080 *state);
void mov_h_e(State8080 *state);
void mov_h_h(State8080 *state);
void mov_h_l(State8080 *state);
void mov_h_m(State8080 *state);
void mov_h_a(State8080 *state);
void mov_l_b(State8080 *state);
void mov_l_c(State8080 *state);
void mov_l_d(State8080 *state);
void mov_l_e(State8080 *state);
void mov_l_h(State8080 *state);
void mov_l_l(State8080 *state);
void mov_l_m(State8080 *state);
void mov_l_a(State8080 *state);

// 0x70
void mov_m_b(State8080 *state);
void mov_m_c(State8080 *state);
void mov_m_d(State8080 *state);
void mov_m_e(State8080 *state);
void mov_m_h(State8080 *state);
void mov_m_l(State8080 *state);
void hlt(State8080 *state);
void mov_m_a(State8080 *state);
void mov_a_b(State8080 *state);
void mov_a_c(State8080 *state);
void mov_a_d(State8080 *state);
void mov_a_e(State8080 *state);
void mov_a_h(State8080 *state);
void mov_a_l(State8080 *state);
void mov_a_m(State8080 *state);
void mov_a_a(State8080 *state);

// 0x80
void add_b(State8080 *state);
void add_c(State8080 *state);
void add_d(State8080 *state);
void add_e(State8080 *state);
void add_h(State8080 *state);
void add_l(State8080 *state);
void add_m(State8080 *state);
void add_a(State8080 *state);
void adc_b(State8080 *state);
void adc_c(State8080 *state);
void adc_d(State8080 *state);
void adc_e(State8080 *state);
void adc_h(State8080 *state);
void adc_l(State8080 *state);
void adc_m(State8080 *state);
void adc_a(State8080 *state);

// 0x90
void sub_b(State8080 *state);
void sub_c(State8080 *state);
void sub_d(State8080 *state);
void sub_e(State8080 *state);
void sub_h(State8080 *state);
void sub_l(State8080 *state);
void sub_m(State8080 *state);
void sub_a(State8080 *state);
void sbb_b(State8080 *state);
void sbb_c(State8080 *state);
void sbb_d(State8080 *state);
void sbb_e(State8080 *state);
void sbb_h(State8080 *state);
void sbb_l(State8080 *state);
void sbb_m(State8080 *state);
void sbb_a(State8080 *state);

// 0xA0
void ana_b(State8080 *state);
void ana_c(State8080 *state);
void ana_d(State8080 *state);
void ana_e(State8080 *state);
void ana_h(State8080 *state);
void ana_l(State8080 *state);
void ana_m(State8080 *state);
void ana_a(State8080 *state);
void xra_b(State8080 *state);
void xra_c(State8080 *state);
void xra_d(State8080 *state);
void xra_e(State8080 *state);
void xra_h(State8080 *state);
void xra_l(State8080 *state);
void xra_m(State8080 *state);
void xra_a(State8080 *state);

// 0xB0
void ora_b(State8080 *state);
void ora_c(State8080 *state);
void ora_d(State8080 *state);
void ora_e(State8080 *state);
void ora_h(State8080 *state);
void ora_l(State8080 *state);
void ora_m(State8080 *state);
void ora_a(State8080 *state);
void cmp_b(State8080 *state);
void cmp_c(State8080 *state);
void cmp_d(State8080 *state);
void cmp_e(State8080 *state);
void cmp_h(State8080 *state);
void cmp_l(State8080 *state);
void cmp_m(State8080 *state);
void cmp_a(State8080 *state);

// 0xC0
void rnz(State8080 *state);
void pop_b(State8080 *state);
void jnz_adr(State8080 *state, uint8_t first, uint8_t second);
void jmp_adr(State8080 *state, uint8_t first, uint8_t second);
void cnz_adr(State8080 *state, uint8_t first, uint8_t second);
void push_b(State8080 *state);
void adi_d8(State8080 *state, uint8_t d8);
void rst_0(State8080 *state);
void rz(State8080 *state);
void ret_adr(State8080 *state, uint8_t first, uint8_t second);
void jz(State8080 *state);
void cz_adr(State8080 *state, uint8_t first, uint8_t second);
void call_adr(State8080 *state, uint8_t first, uint8_t second);
void aci_d8(State8080 *state, uint8_t d8);
void rst_1(State8080 *state);

// 0xD0
void rnc(State8080 *state);
void pop_d(State8080 *state);
void jnc_adr(State8080 *state, uint8_t first, uint8_t second);
void out_d8(State8080 *state, uint8_t d8);
void cnc_adr(State8080 *state, uint8_t first, uint8_t second);
void push_d(State8080 *state);
void sui_d8(State8080 *state, uint8_t d8);
void rst_2(State8080 *state);
void rc(State8080 *state);
void jc_adr(State8080 *state, uint8_t first, uint8_t second);
void in_d8(State8080 *state, uint8_t d8);
void cc_adr(State8080 *state, uint8_t first, uint8_t second);
void sbi_d8(State8080 *state, uint8_t d8);
void rst_3(State8080 *state);

// 0xE0
void rpo(State8080 *state);
void pop_h(State8080 *state);
void jpo_adr(State8080 *state, uint8_t first, uint8_t second);
void xthl(State8080 *state);
void cpo_adr(State8080 *state, uint8_t first, uint8_t second);
void push_h(State8080 *state);
void ani_d8(State8080 *state, uint8_t d8);
void rst_4(State8080 *state);
void rpe(State8080 *state);
void pchl(State8080 *state);
void jpe_adr(State8080 *state, uint8_t first, uint8_t second);
void xchg(State8080 *state);
void cpe_adr(State8080 *state, uint8_t first, uint8_t second);
void xri_d8(State8080 *state, uint8_t d8);
void rst_5(State8080 *state);

// 0xF0
void rp(State8080 *state);
void pop_psw(State8080 *state);
void jp_adr(State8080 *state, uint8_t first, uint8_t second);
void di(State8080 *state);
void cp_adr(State8080 *state, uint8_t first, uint8_t second);
void push_psw(State8080 *state);
void ori_d8(State8080 *state, uint8_t d8);
void rst_6(State8080 *state);
void rm(State8080 *state);
void sphl(State8080 *state);
void jm_adr(State8080 *state, uint8_t first, uint8_t second);
void ei(State8080 *state);
void cm_adr(State8080 *state, uint8_t first, uint8_t second);
void cpi_d8(State8080 *state, uint8_t d8);
void rst_7(State8080 *state);

#endif