#include "../include/8080emu.h"

/**
 * 0x00 - 0x0F
 */

// Loads 16bits 
void lxi_b_d16(State8080 *self, uint8_t low, uint8_t high)
{
	self->c = low;
	self->b = high;
	self->pc += 2;
}

void stax_b(State8080 *self)
{
	self->memory[self->bc] = self->a;
}

void inx_b(State8080 *self)
{
	self->bc++;
}

void inr_b(State8080 *self)
{
	self->b++;
	flagsZSP(self, self->b);
	flagAC(self, self->b, 1);
}

void dcr_b(State8080 *self)
{
	self->b--;
	flagsZSP(self, self->b);
	flagAC(self, self->b, -1);
}

void mvi_b_d8(State8080 *self, uint8_t d8)
{
	self->b = d8;
	self->pc++;
}

//Not sure if this works right. Probably test the logic.
void rlc(State8080 *self)
{
	uint8_t bit_7 = (0x80 == (self->a & 0x80));
	self->a <<= 1;
	self->a = self->a | bit_7;
	self->cc.cy = bit_7;
}

void dad_b(State8080 *self)
{
	self->hl += self->bc;
	self->cc.cy = ((uint32_t)(self->hl + self->bc) > 0xffff);
}

void ldax_b(State8080 *self)
{
	self->a = self->memory[self->bc];
}

// Decrements BC register by 1
void dcx_b(State8080 *self)
{
	self->bc--;
}

// Increment register C by 1
// Flags: Z, S, P, AC
void inr_c(State8080 *self)
{
	self->c++;
	flagsZSP(self, self->c);
	flagAC(self, self->c, 1);
}

// Decrement register C by 1
// Flags: Z, S, P, AC
void dcr_c(State8080 *self)
{
	self->c--;
	flagsZSP(self, self->c);
	flagAC(self, self->c, -1);
}

void mvi_c_d8(State8080 *self, uint8_t d8)
{
	self->c = d8;
	self->pc++;
}

// Think this through again
void rrc(State8080 *self)
{
	uint8_t bit_0 = (self->a & 0x1);
	self->a >>= 1;
	self->cc.cy = bit_0;
	bit_0 <<= 7;
	self->a = self->a | bit_0;
}



/**
 * 0x10 - 0x1F
 */

void lxi_d_d16(State8080 *self, uint8_t low, uint8_t high)
{
	self->e = low;
	self->d = high;
	self->pc += 2;
}

void stax_d(State8080 *self)
{
	self->memory[self->de] = self->a;
}

void inx_d(State8080 *self)
{
	self->de++;
}

void inr_d(State8080 *self)
{
	self->d++;
	flagsZSP(self, self->d);
	flagAC(self, self->d, 1);
}

void dcr_d(State8080 *self)
{
	self->d--;
	flagsZSP(self, self->d);
	flagAC(self, self->d, -1);
}

void mvi_d_d8(State8080 *self, uint8_t d8)
{
	self->d = d8;
	self->pc++;
}

void ral(State8080 *self)
{
	uint8_t bit_7 = (0x80 == (0x80 & self->a));
	self->a <<= 1;
	self->a = self->a | self->cc.cy;
	self->cc.cy = bit_7;
}

void dad_d(State8080 *self)
{
	self->hl = self->hl + self->de;
	self->cc.cy = ((uint32_t)(self->hl + self->de) > 0xffff);
}

void ldax_d(State8080 *self)
{
	self->a = self->memory[self->de];
}

void dcx_d(State8080 *self)
{
	self->de--;
}

void inr_e(State8080 *self)
{
	self->e++;
	flagsZSP(self, self->e);
	flagAC(self, self->e, 1);
}

void dcr_e(State8080 *self)
{
	self->e--;
	flagsZSP(self, self->e);
	flagAC(self, self->e, -1);
}

void mvi_e_d8(State8080 *self, uint8_t d8)
{
	self->e = d8;
	self->pc++;
}

// Did I get this right?
void rar(State8080 *self)
{
	uint8_t bit_0 = (self->a & 0x1);
	self->a >>= 1;
	self->a = self->a | (self->cc.cy << 7);
	self->cc.cy = bit_0;
}



/**
 * 0x20 - 0x2F
 */

void lxi_h_d16(State8080 *self, uint8_t low, uint8_t high)
{
	self->l = low;
	self->h = high;
	self->pc += 2;
}

// Is this correct?
void shld_adr(State8080 *self, uint8_t low, uint8_t high)
{
	uint16_t address = low | (high << 8);
	self->memory[address] = self->l;
	self->memory[address + 1] = self->h;
	self->pc += 2;
}

void inx_h(State8080 *self)
{
	self->hl++;
}

void inr_h(State8080 *self)
{
	self->h++;
	flagsZSP(self, self->h);
	flagAC(self, self->h, 1);
}

void dcr_h(State8080 *self)
{
	self->h--;
	flagsZSP(self, self->h);
	flagAC(self, self->h, -1);
}

void mvi_h_d8(State8080 *self, uint8_t d8)
{
	self->h = d8;
	self->pc++;
}

void daa(State8080 *self)
{
	// ?
}

void dad_h(State8080 *self)
{
	self->cc.cy = (0x80 == (0x80 & self->hl));
	// Apparently, adding two 16-bit numbers to itself is equivalent to 
	// shifting it to the left by one bit. 
	// Original should be: self->hl = self->hl + self->hl;
	self->h <<= 1;
}

void lhld_adr(State8080 *self, uint8_t low, uint8_t high)
{
	self->l = low;
	self->h = high;
	self->pc += 2;
}

void dcx_h(State8080 *self)
{
	self->hl--;
}

void inr_l(State8080 *self)
{
	self->l++;
	flagsZSP(self, self->l);
	flagAC(self, self->l, 1);
}

void dcr_l(State8080 *self)
{
	self->l--;
	flagsZSP(self, self->l);
	flagAC(self, self->l, -1);
}

void mvi_l_d8(State8080 *self, uint8_t d8)
{
	self->l = d8;
	self->pc++;
}

void cma(State8080 *self)
{
	self->a = ~self->a;
}



/**
 * 0x30 - 0x3F
 */

void lxi_sp_d16(State8080 *self, uint8_t low, uint8_t high)
{
	self->sp = high << 8 | low;
}

void sta_adr(State8080 *self, uint8_t low, uint8_t high)
{
	uint16_t address = high << 8 | low;
	self->memory[address] = self->a;
}

void inx_sp(State8080 *self)
{

}

void inr_m(State8080 *self)
{

}

void dcr_m(State8080 *self)
{

}

void mvi_m_d8(State8080 *self, uint8_t d8)
{

}

void stc(State8080 *self)
{

}

void dad_sp(State8080 *self)
{

}

void lda_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void dcx_sp(State8080 *self)
{

}

void inr_a(State8080 *self)
{

}

void dcr_a(State8080 *self)
{

}

void mvi_a_d8(State8080 *self, uint8_t d8)
{

}

void cmc(State8080 *self)
{

}



/**
 * 0x40 - 0x4F
 */

void mov_b_b(State8080 *self)
{

}

void mov_b_c(State8080 *self)
{

}

void mov_b_d(State8080 *self)
{

}

void mov_b_e(State8080 *self)
{

}

void mov_b_h(State8080 *self)
{

}

void mov_b_l(State8080 *self)
{

}

void mov_b_m(State8080 *self)
{

}

void mov_b_a(State8080 *self)
{

}

void mov_c_b(State8080 *self)
{

}

void mov_c_c(State8080 *self)
{

}

void mov_c_d(State8080 *self)
{

}

void mov_c_e(State8080 *self)
{

}

void mov_c_h(State8080 *self)
{

}

void mov_c_l(State8080 *self)
{

}

void mov_c_m(State8080 *self)
{

}

void mov_c_a(State8080 *self)
{

}



/**
 * 0x50 - 0x5F
 */

void mov_d_b(State8080 *self)
{

}

void mov_d_c(State8080 *self)
{

}

void mov_d_d(State8080 *self)
{

}

void mov_d_e(State8080 *self)
{

}

void mov_d_h(State8080 *self)
{

}

void mov_d_l(State8080 *self)
{

}

void mov_d_m(State8080 *self)
{

}

void mov_d_a(State8080 *self)
{

}

void mov_e_b(State8080 *self)
{

}

void mov_e_c(State8080 *self)
{

}

void mov_e_d(State8080 *self)
{

}

void mov_e_e(State8080 *self)
{

}

void mov_e_h(State8080 *self)
{

}

void mov_e_l(State8080 *self)
{

}

void mov_e_m(State8080 *self)
{

}

void mov_e_a(State8080 *self)
{

}



/**
 * 0x60 - 0x6F
 */

void mov_h_b(State8080 *self)
{

}

void mov_h_c(State8080 *self)
{

}

void mov_h_d(State8080 *self)
{

}

void mov_h_e(State8080 *self)
{

}

void mov_h_h(State8080 *self)
{

}

void mov_h_l(State8080 *self)
{

}

void mov_h_m(State8080 *self)
{

}

void mov_h_a(State8080 *self)
{

}

void mov_l_b(State8080 *self)
{

}

void mov_l_c(State8080 *self)
{

}

void mov_l_d(State8080 *self)
{

}

void mov_l_e(State8080 *self)
{

}

void mov_l_h(State8080 *self)
{

}

void mov_l_l(State8080 *self)
{

}

void mov_l_m(State8080 *self)
{

}

void mov_l_a(State8080 *self)
{

}



/**
 * 0x70 - 0x7F
 */

void mov_m_b(State8080 *self)
{

}

void mov_m_c(State8080 *self)
{

}

void mov_m_d(State8080 *self)
{

}

void mov_m_e(State8080 *self)
{

}

void mov_m_h(State8080 *self)
{

}

void mov_m_l(State8080 *self)
{

}

void hlt(State8080 *self)
{

}

void mov_m_a(State8080 *self)
{

}

void mov_a_b(State8080 *self)
{

}

void mov_a_c(State8080 *self)
{

}

void mov_a_d(State8080 *self)
{

}

void mov_a_e(State8080 *self)
{

}

void mov_a_h(State8080 *self)
{

}

void mov_a_l(State8080 *self)
{

}

void mov_a_m(State8080 *self)
{

}

void mov_a_a(State8080 *self)
{

}



/**
 * 0x80 - 0x8F
 */

void add_b(State8080 *self)
{

}

void add_c(State8080 *self)
{

}

void add_d(State8080 *self)
{

}

void add_e(State8080 *self)
{

}

void add_h(State8080 *self)
{

}

void add_l(State8080 *self)
{

}

void add_m(State8080 *self)
{

}

void add_a(State8080 *self)
{

}

void adc_b(State8080 *self)
{

}

void adc_c(State8080 *self)
{

}

void adc_d(State8080 *self)
{

}

void adc_e(State8080 *self)
{

}

void adc_h(State8080 *self)
{

}

void adc_l(State8080 *self)
{

}

void adc_m(State8080 *self)
{

}

void adc_a(State8080 *self)
{

}



/**
 * 0x90 - 0x9F
 */

void sub_b(State8080 *self)
{

}

void sub_c(State8080 *self)
{

}

void sub_d(State8080 *self)
{

}

void sub_e(State8080 *self)
{

}

void sub_h(State8080 *self)
{

}

void sub_l(State8080 *self)
{

}

void sub_m(State8080 *self)
{

}

void sub_a(State8080 *self)
{

}

void sbb_b(State8080 *self)
{

}

void sbb_c(State8080 *self)
{

}

void sbb_d(State8080 *self)
{

}

void sbb_e(State8080 *self)
{

}

void sbb_h(State8080 *self)
{

}

void sbb_l(State8080 *self)
{

}

void sbb_m(State8080 *self)
{

}

void sbb_a(State8080 *self)
{

}



/**
 * 0xA0 - 0xAF
 */

void ana_b(State8080 *self)
{

}

void ana_c(State8080 *self)
{

}

void ana_d(State8080 *self)
{

}

void ana_e(State8080 *self)
{

}

void ana_h(State8080 *self)
{

}

void ana_l(State8080 *self)
{

}

void ana_m(State8080 *self)
{

}

void ana_a(State8080 *self)
{

}

void xra_b(State8080 *self)
{

}

void xra_c(State8080 *self)
{

}

void xra_d(State8080 *self)
{

}

void xra_e(State8080 *self)
{

}

void xra_h(State8080 *self)
{

}

void xra_l(State8080 *self)
{

}

void xra_m(State8080 *self)
{

}

void xra_a(State8080 *self)
{

}



/**
 * 0xB0 - 0xBF
 */

void ora_b(State8080 *self)
{

}

void ora_c(State8080 *self)
{

}

void ora_d(State8080 *self)
{

}

void ora_e(State8080 *self)
{

}

void ora_h(State8080 *self)
{

}

void ora_l(State8080 *self)
{

}

void ora_m(State8080 *self)
{

}

void ora_a(State8080 *self)
{

}

void cmp_b(State8080 *self)
{

}

void cmp_c(State8080 *self)
{

}

void cmp_d(State8080 *self)
{

}

void cmp_e(State8080 *self)
{

}

void cmp_h(State8080 *self)
{

}

void cmp_l(State8080 *self)
{

}

void cmp_m(State8080 *self)
{

}

void cmp_a(State8080 *self)
{

}



/**
 * 0xC0 - 0xCF
 */

void rnz(State8080 *self)
{

}

void pop_b(State8080 *self)
{

}

void jnz_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void jmp_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void cnz_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void push_b(State8080 *self)
{

}

void adi_d8(State8080 *self, uint8_t d8)
{

}

void rst_0(State8080 *self)
{

}

void rz(State8080 *self)
{

}

void ret_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void jz(State8080 *self)
{

}

void cz_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void call_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void aci_d8(State8080 *self, uint8_t d8)
{

}

void rst_1(State8080 *self)
{

}



/**
 * 0xD0 - 0xDF
 */

void rnc(State8080 *self)
{

}

void pop_d(State8080 *self)
{

}

void jnc_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void out_d8(State8080 *self, uint8_t d8)
{

}

void cnc_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void push_d(State8080 *self)
{

}

void sui_d8(State8080 *self, uint8_t d8)
{

}

void rst_2(State8080 *self)
{

}

void rc(State8080 *self)
{

}

void jc_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void in_d8(State8080 *self, uint8_t d8)
{

}

void cc_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void sbi_d8(State8080 *self, uint8_t d8)
{

}

void rst_3(State8080 *self)
{

}



/**
 * 0xE0 - 0xEF
 */

void rpo(State8080 *self)
{

}

void pop_h(State8080 *self)
{

}

void jpo_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void xthl(State8080 *self)
{

}

void cpo_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void push_h(State8080 *self)
{

}

void ani_d8(State8080 *self, uint8_t d8)
{

}

void rst_4(State8080 *self)
{

}

void rpe(State8080 *self)
{

}

void pchl(State8080 *self)
{

}

void jpe_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void xchg(State8080 *self)
{

}

void cpe_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void xri_d8(State8080 *self, uint8_t d8)
{

}

void rst_5(State8080 *self)
{

}



/**
 * 0xF0 - 0xFF
 */

void rp(State8080 *self)
{

}

void pop_psw(State8080 *self)
{

}

void jp_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void di(State8080 *self)
{

}

void cp_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void push_psw(State8080 *self)
{

}

void ori_d8(State8080 *self, uint8_t d8)
{

}

void rst_6(State8080 *self)
{

}

void rm(State8080 *self)
{

}

void sphl(State8080 *self)
{

}

void jm_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void ei(State8080 *self)
{

}

void cm_adr(State8080 *self, uint8_t low, uint8_t high)
{

}

void cpi_d8(State8080 *self, uint8_t d8)
{

}

void rst_7(State8080 *self)
{

}
