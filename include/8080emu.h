#ifndef _8080EMU_H
#define _8080EMU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_MEMORY 0x10000

typedef struct ConditionCodes ConditionCodes;
typedef struct State8080 State8080;


/**
 * These are the condition bits (or flags)
 * 
 * NOTE: When using this as a variable, the bits are in reverse order.
 * s is the highest order, cy is the lowest order.
 * This can be accessed as the PSW byte
 */
struct ConditionCodes {
	// Space invaders doesn't use ac	
	uint8_t cy:1, BIT_1:1, p:1, BIT_3:1, ac:1, BIT_5:1, z:1, s:1;
};

struct State8080 {
	uint8_t a, int_enable, pc_inc, memory[MAX_MEMORY];
	uint16_t sp, pc, rom;
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

/**
 * 
 * 		Non-Opcode Function Prototypes
 * 
 */

// Debugging function.
void unimplementedInstruction(uint8_t code);

void init_8080(State8080 *self, FILE* file);

uint16_t address_concat(uint8_t low, uint8_t high);

int parity(uint8_t value);
void flagsZSP(State8080 *self, uint8_t value);
void flagAC(State8080 *self, uint8_t initial, uint16_t added);		// CY flag is set in the opcode logic, since it's different by opcode

int disassemble8080Op(uint8_t *codebuffer, int pc);
void emulate8080Op(State8080 *self);





/**
 * 
 * 		Opcode Function Prototypes
 * 		Defined in 8080ops.h
 * 
 */

// 0x00 - 0x0F
void lxi_b_d16(State8080 *self, uint8_t low, uint8_t high);
void stax_b(State8080 *self);
void inx_b(State8080 *self);
void inr_b(State8080 *self);
void dcr_b(State8080 *self);
void mvi_b_d8(State8080 *self, uint8_t d8);
void rlc(State8080 *self);
void dad_b(State8080 *self);
void ldax_b(State8080 *self);
void dcx_b(State8080 *self);
void inr_c(State8080 *self);
void dcr_c(State8080 *self);
void mvi_c_d8(State8080 *self, uint8_t d8);
void rrc(State8080 *self);

// 0x10 - 0x1F
void lxi_d_d16(State8080 *self, uint8_t low, uint8_t high);
void stax_d(State8080 *self);
void inx_d(State8080 *self);
void inr_d(State8080 *self);
void dcr_d(State8080 *self);
void mvi_d_d8(State8080 *self, uint8_t d8);
void ral(State8080 *self);
void dad_d(State8080 *self);
void ldax_d(State8080 *self);
void dcx_d(State8080 *self);
void inr_e(State8080 *self);
void dcr_e(State8080 *self);
void mvi_e_d8(State8080 *self, uint8_t d8);
void rar(State8080 *self);

// 0x20 - 0x2F
void lxi_h_d16(State8080 *self, uint8_t low, uint8_t high);
void shld_adr(State8080 *self, uint8_t low, uint8_t high);
void inx_h(State8080 *self);
void inr_h(State8080 *self);
void dcr_h(State8080 *self);
void mvi_h_d8(State8080 *self, uint8_t d8);
void daa(State8080 *self);
void dad_h(State8080 *self);
void lhld_adr(State8080 *self, uint8_t low, uint8_t high);
void dcx_h(State8080 *self);
void inr_l(State8080 *self);
void dcr_l(State8080 *self);
void mvi_l_d8(State8080 *self, uint8_t d8);
void cma(State8080 *self);

// 0x30 - 0x3F
void lxi_sp_d16(State8080 *self, uint8_t low, uint8_t high);
void sta_adr(State8080 *self, uint8_t low, uint8_t high);
void inx_sp(State8080 *self);
void inr_m(State8080 *self);
void dcr_m(State8080 *self);
void mvi_m_d8(State8080 *self, uint8_t d8);
void stc(State8080 *self);
void dad_sp(State8080 *self);
void lda_adr(State8080 *self, uint8_t low, uint8_t high);
void dcx_sp(State8080 *self);
void inr_a(State8080 *self);
void dcr_a(State8080 *self);
void mvi_a_d8(State8080 *self, uint8_t d8);
void cmc(State8080 *self);

// 0x40 - 0x4F
void mov_b_b(State8080 *self);
void mov_b_c(State8080 *self);
void mov_b_d(State8080 *self);
void mov_b_e(State8080 *self);
void mov_b_h(State8080 *self);
void mov_b_l(State8080 *self);
void mov_b_m(State8080 *self);
void mov_b_a(State8080 *self);
void mov_c_b(State8080 *self);
void mov_c_c(State8080 *self);
void mov_c_d(State8080 *self);
void mov_c_e(State8080 *self);
void mov_c_h(State8080 *self);
void mov_c_l(State8080 *self);
void mov_c_m(State8080 *self);
void mov_c_a(State8080 *self);

// 0x50 - 0x5F
void mov_d_b(State8080 *self);
void mov_d_c(State8080 *self);
void mov_d_d(State8080 *self);
void mov_d_e(State8080 *self);
void mov_d_h(State8080 *self);
void mov_d_l(State8080 *self);
void mov_d_m(State8080 *self);
void mov_d_a(State8080 *self);
void mov_e_b(State8080 *self);
void mov_e_c(State8080 *self);
void mov_e_d(State8080 *self);
void mov_e_e(State8080 *self);
void mov_e_h(State8080 *self);
void mov_e_l(State8080 *self);
void mov_e_m(State8080 *self);
void mov_e_a(State8080 *self);

// 0x60 - 0x6F
void mov_h_b(State8080 *self);
void mov_h_c(State8080 *self);
void mov_h_d(State8080 *self);
void mov_h_e(State8080 *self);
void mov_h_h(State8080 *self);
void mov_h_l(State8080 *self);
void mov_h_m(State8080 *self);
void mov_h_a(State8080 *self);
void mov_l_b(State8080 *self);
void mov_l_c(State8080 *self);
void mov_l_d(State8080 *self);
void mov_l_e(State8080 *self);
void mov_l_h(State8080 *self);
void mov_l_l(State8080 *self);
void mov_l_m(State8080 *self);
void mov_l_a(State8080 *self);

// 0x70 - 0x7F
void mov_m_b(State8080 *self);
void mov_m_c(State8080 *self);
void mov_m_d(State8080 *self);
void mov_m_e(State8080 *self);
void mov_m_h(State8080 *self);
void mov_m_l(State8080 *self);
void hlt(State8080 *self);
void mov_m_a(State8080 *self);
void mov_a_b(State8080 *self);
void mov_a_c(State8080 *self);
void mov_a_d(State8080 *self);
void mov_a_e(State8080 *self);
void mov_a_h(State8080 *self);
void mov_a_l(State8080 *self);
void mov_a_m(State8080 *self);
void mov_a_a(State8080 *self);

// 0x80 - 0x8F
void add_b(State8080 *self);
void add_c(State8080 *self);
void add_d(State8080 *self);
void add_e(State8080 *self);
void add_h(State8080 *self);
void add_l(State8080 *self);
void add_m(State8080 *self);
void add_a(State8080 *self);
void adc_b(State8080 *self);
void adc_c(State8080 *self);
void adc_d(State8080 *self);
void adc_e(State8080 *self);
void adc_h(State8080 *self);
void adc_l(State8080 *self);
void adc_m(State8080 *self);
void adc_a(State8080 *self);

// 0x90 - 0x9F
void sub_b(State8080 *self);
void sub_c(State8080 *self);
void sub_d(State8080 *self);
void sub_e(State8080 *self);
void sub_h(State8080 *self);
void sub_l(State8080 *self);
void sub_m(State8080 *self);
void sub_a(State8080 *self);
void sbb_b(State8080 *self);
void sbb_c(State8080 *self);
void sbb_d(State8080 *self);
void sbb_e(State8080 *self);
void sbb_h(State8080 *self);
void sbb_l(State8080 *self);
void sbb_m(State8080 *self);
void sbb_a(State8080 *self);

// 0xA0 - 0xAF
void ana_b(State8080 *self);
void ana_c(State8080 *self);
void ana_d(State8080 *self);
void ana_e(State8080 *self);
void ana_h(State8080 *self);
void ana_l(State8080 *self);
void ana_m(State8080 *self);
void ana_a(State8080 *self);
void xra_b(State8080 *self);
void xra_c(State8080 *self);
void xra_d(State8080 *self);
void xra_e(State8080 *self);
void xra_h(State8080 *self);
void xra_l(State8080 *self);
void xra_m(State8080 *self);
void xra_a(State8080 *self);

// 0xB0 - 0xBF
void ora_b(State8080 *self);
void ora_c(State8080 *self);
void ora_d(State8080 *self);
void ora_e(State8080 *self);
void ora_h(State8080 *self);
void ora_l(State8080 *self);
void ora_m(State8080 *self);
void ora_a(State8080 *self);
void cmp_b(State8080 *self);
void cmp_c(State8080 *self);
void cmp_d(State8080 *self);
void cmp_e(State8080 *self);
void cmp_h(State8080 *self);
void cmp_l(State8080 *self);
void cmp_m(State8080 *self);
void cmp_a(State8080 *self);

// 0xC0 - 0xCF
void rnz(State8080 *self);
void pop_b(State8080 *self);
void jnz_adr(State8080 *self, uint8_t low, uint8_t high);
void jmp_adr(State8080 *self, uint8_t low, uint8_t high);
void cnz_adr(State8080 *self, uint8_t low, uint8_t high);
void push_b(State8080 *self);
void adi_d8(State8080 *self, uint8_t d8);
void rst_0(State8080 *self);
void rz(State8080 *self);
void ret(State8080 *self);
void jz_adr(State8080 *self, uint8_t low, uint8_t high);
void cz_adr(State8080 *self, uint8_t low, uint8_t high);
void call_adr(State8080 *self, uint8_t low, uint8_t high);
void aci_d8(State8080 *self, uint8_t d8);
void rst_1(State8080 *self);

// 0xD0 - 0xDF
void rnc(State8080 *self);
void pop_d(State8080 *self);
void jnc_adr(State8080 *self, uint8_t low, uint8_t high);
void out_d8(State8080 *self, uint8_t d8);
void cnc_adr(State8080 *self, uint8_t low, uint8_t high);
void push_d(State8080 *self);
void sui_d8(State8080 *self, uint8_t d8);
void rst_2(State8080 *self);
void rc(State8080 *self);
void jc_adr(State8080 *self, uint8_t low, uint8_t high);
void in_d8(State8080 *self, uint8_t d8);
void cc_adr(State8080 *self, uint8_t low, uint8_t high);
void sbi_d8(State8080 *self, uint8_t d8);
void rst_3(State8080 *self);

// 0xE0 - 0xEF
void rpo(State8080 *self);
void pop_h(State8080 *self);
void jpo_adr(State8080 *self, uint8_t low, uint8_t high);
void xthl(State8080 *self);
void cpo_adr(State8080 *self, uint8_t low, uint8_t high);
void push_h(State8080 *self);
void ani_d8(State8080 *self, uint8_t d8);
void rst_4(State8080 *self);
void rpe(State8080 *self);
void pchl(State8080 *self);
void jpe_adr(State8080 *self, uint8_t low, uint8_t high);
void xchg(State8080 *self);
void cpe_adr(State8080 *self, uint8_t low, uint8_t high);
void xri_d8(State8080 *self, uint8_t d8);
void rst_5(State8080 *self);

// 0xF0 - 0xFF
void rp(State8080 *self);
void pop_psw(State8080 *self);
void jp_adr(State8080 *self, uint8_t low, uint8_t high);
void di(State8080 *self);
void cp_adr(State8080 *self, uint8_t low, uint8_t high);
void push_psw(State8080 *self);
void ori_d8(State8080 *self, uint8_t d8);
void rst_6(State8080 *self);
void rm(State8080 *self);
void sphl(State8080 *self);
void jm_adr(State8080 *self, uint8_t low, uint8_t high);
void ei(State8080 *self);
void cm_adr(State8080 *self, uint8_t low, uint8_t high);
void cpi_d8(State8080 *self, uint8_t d8);
void rst_7(State8080 *self);

#endif