#include "../include/8080emu.h"

#define CPUDIAG

/**
 * Helper Functions
 */

// Adds the register/byte to the accumulator
// Flags Affected: z, s, p, ac, cy
static void add(State8080 *self, uint8_t byte)
{
	// Carry flags
	self->cc.cy = ((uint16_t)(self->a + byte) > 0xff);
	flagAC(self, self->a, byte);

	self->a += byte;

	flagsZSP(self, self->a);
}

// Adds the register/byte and carry bit to the accumulator
// Flags Affected: z, s, p, ac, cy
static void adc(State8080 *self, uint8_t byte)
{
	byte += self->cc.cy;	// Adds contents of carry to register/byte

	add(self, byte);
}

// Subtracts the register/byte from the accumulator
// Flags Affected: z, s, p, ac, cy
static void sub(State8080 *self, uint8_t byte)
{
	uint8_t tc = ~byte + 1;	// Gets the two's complement

	// Carry flags
	self->cc.cy = !((uint16_t)(self->a + tc) > 0xff);
	flagAC(self, self->a, tc);

	self->a -= byte;

	flagsZSP(self, self->a);
}

// Subtracts the register/byte and carry bit from the accumulator
// Flags Affected: z, s, p, ac, cy
static void sbb(State8080 *self, uint8_t byte)
{
	byte += self->cc.cy;	// Adds contents of carry to register byte
	
	sub(self, byte);
}

// The accumulator is bitwise ANDed with the register
// Flags Affected: z, s, p, cy
static void ana(State8080 *self, uint8_t byte)
{
	self->a &= byte;

	self->cc.cy = 0;
	// self->cc.ac = 0;	AC not affected?
	flagsZSP(self, self->a);
}

// The accumulator is bitwise XORed with the register/byte
// Flags Affected: z, s, p, ac, cy
static void xra(State8080 *self, uint8_t byte)
{
	self->a ^= byte;

	// Flags
	self->cc.cy = 0;
	self->cc.ac = 0;	// This isn't in the docs, it's an assumption
	flagsZSP(self, self->a);
}

// The accumulator is bitwise ORed with the register/byte
// Flags Affected: z, s, p, cy
static void ora(State8080 *self, uint8_t byte)
{
	self->a |= byte;

	// Flags
	self->cc.cy = 0;
	self->cc.ac = 0;
	flagsZSP(self, self->a);
}

// Internally, the register/byte is subtracted from the accumulator.
// The flags are then set based on the subtraction
// Zero being set means they're equal
//
// Assuming they have the same sign:
// Then a carry bit being 0 means the accumulator is greater
// If the carry bit is 1 that means the accumulator is smaller
//
// Reverse that if the signs are opposite
//
// Flags Affected: z, s, p, ac, cy
static void cmp(State8080 *self, uint8_t byte)
{
	uint8_t tc, result;
	
	// Carry flags
	tc = ~byte + 1;
	//self->cc.cy = !((uint16_t)(self->a + tc) > 0xff);
	self->cc.cy = self->a < byte;
	flagAC(self, self->a, tc);
	
	result = self->a - byte;

	flagsZSP(self, result);
}

// Pushes two halves of a 16 bit register onto stack
// Flags affected: None
static void push(State8080 *self, uint8_t high, uint8_t low)
{
	self->memory[self->sp - 1] = high;
	self->memory[self->sp - 2] = low;

	self->sp -= 2;
}

// Pops two 8 bit values into given register
// Flags affected: None, unless PSW is selected for popping
//		   In that case, they all may be affected
static void pop(State8080 *self, uint8_t *high, uint8_t *low)
{
	*low = self->memory[self->sp];
	*high = self->memory[self->sp + 1];

	self->sp += 2;
}

// Double add: Adds the given 16bit register to the HL register
// Flags affected: Carry
static void dad(State8080 *self, uint16_t byte)
{
	self->cc.cy = ((uint32_t)(self->hl + byte) > 0xffff);
	self->hl += byte;
}

// Will jump to 
// Flags affected: None
static void jmp_if(State8080 *self, uint8_t condition, uint8_t low, uint8_t high)
{
	if (condition) {
		uint16_t address = address_concat(low, high);

		self->pc = address;
		self->pc_inc = 0;
	} else {
		self->pc_inc = 3;
	}
}





/**
 * 0x00 - 0x0F
 */

// Loads 16bits 
void lxi_b_d16(State8080 *self, uint8_t low, uint8_t high)
{
	self->c = low;
	self->b = high;
	self->pc_inc = 3;
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
	flagAC(self, self->b, 1);
	self->b++;
	flagsZSP(self, self->b);
}

void dcr_b(State8080 *self)
{
	flagAC(self, self->b, -1);
	self->b--;
	flagsZSP(self, self->b);
}

void mvi_b_d8(State8080 *self, uint8_t d8)
{
	self->b = d8;
	self->pc_inc = 2;
}

void rlc(State8080 *self)
{
	uint8_t bit_7 = (0x80 == (self->a & 0x80));
	self->a <<= 1;
	self->a = self->a | bit_7;
	self->cc.cy = bit_7;
}

void dad_b(State8080 *self)
{
	dad(self, self->bc);
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
	flagAC(self, self->c, 1);
	self->c++;
	flagsZSP(self, self->c);
}

// Decrement register C by 1
// Flags: Z, S, P, AC
void dcr_c(State8080 *self)
{
	flagAC(self, self->c, -1);
	self->c--;
	flagsZSP(self, self->c);
}

void mvi_c_d8(State8080 *self, uint8_t d8)
{
	self->c = d8;
	self->pc_inc = 2;
}

void rrc(State8080 *self)
{
	uint8_t bit_0 = (self->a & 0x1);
	self->a >>= 1;
	self->a = self->a | (bit_0 << 7);
	self->cc.cy = bit_0;
}



/**
 * 0x10 - 0x1F
 */

void lxi_d_d16(State8080 *self, uint8_t low, uint8_t high)
{
	self->e = low;
	self->d = high;
	self->pc_inc = 3;
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
	flagAC(self, self->d, 1);
	self->d++;
	flagsZSP(self, self->d);
}

void dcr_d(State8080 *self)
{
	flagAC(self, self->d, -1);
	self->d--;
	flagsZSP(self, self->d);
}

void mvi_d_d8(State8080 *self, uint8_t d8)
{
	self->d = d8;
	self->pc_inc = 2;
}

void ral(State8080 *self)
{
	uint8_t bit_7 = (0x80 == (self->a & 0x80));
	self->a <<= 1;
	self->a = self->a | self->cc.cy;
	self->cc.cy = bit_7;
}

void dad_d(State8080 *self)
{
	dad(self, self->de);
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
	flagAC(self, self->e, 1);
	self->e++;
	flagsZSP(self, self->e);
}

void dcr_e(State8080 *self)
{
	flagAC(self, self->e, -1);
	self->e--;
	flagsZSP(self, self->e);
}

void mvi_e_d8(State8080 *self, uint8_t d8)
{
	self->e = d8;
	self->pc_inc = 2;
}

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
	self->pc_inc = 3;
}

// Is this correct?
void shld_adr(State8080 *self, uint8_t low, uint8_t high)
{
	uint16_t address = address_concat(low, high);

	self->memory[address] = self->l;
	self->memory[address + 1] = self->h;

	self->pc_inc = 3;
}

void inx_h(State8080 *self)
{
	self->hl++;
}

void inr_h(State8080 *self)
{
	flagAC(self, self->h, 1);
	self->h++;
	flagsZSP(self, self->h);
}

void dcr_h(State8080 *self)
{
	flagAC(self, self->h, -1);
	self->h--;
	flagsZSP(self, self->h);
}

void mvi_h_d8(State8080 *self, uint8_t d8)
{
	self->h = d8;
	self->pc_inc = 2;
}

// Only instruction affected by Auxiliary Carry
void daa(State8080 *self)
{
	uint8_t lsb, msb;

	lsb = self->a & 0xf;

	if (lsb > 9 || self->cc.ac == 1) {
		self->cc.ac = (lsb + 6) > 0xf;
		self->a += 6;
	}

	msb = self->a & 0xf0;

	if (msb > (9 << 4) || self->cc.cy == 1) {
		self->cc.cy = ((uint16_t)(msb + (6 << 4)) > 0xff);
		self->a += (6 << 4);
	}

	flagsZSP(self, self->a);
}

void dad_h(State8080 *self)
{
	dad(self, self->hl);
}

void lhld_adr(State8080 *self, uint8_t low, uint8_t high)
{
	uint16_t address = address_concat(low, high);

	self->l = self->memory[address];
	self->h = self->memory[address + 1];

	self->pc_inc = 3;
}

void dcx_h(State8080 *self)
{
	self->hl--;
}

void inr_l(State8080 *self)
{
	flagAC(self, self->l, 1);
	self->l++;
	flagsZSP(self, self->l);
}

void dcr_l(State8080 *self)
{
	flagAC(self, self->l, -1);
	self->l--;
	flagsZSP(self, self->l);
}

void mvi_l_d8(State8080 *self, uint8_t d8)
{
	self->l = d8;
	self->pc_inc = 2;
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
	self->sp = address_concat(low, high);
	self->pc_inc = 3;
}

void sta_adr(State8080 *self, uint8_t low, uint8_t high)
{
	uint16_t address = address_concat(low, high);
	self->memory[address] = self->a;

	self->pc_inc = 3;
}

void inx_sp(State8080 *self)
{
	self->sp++;
}

void inr_m(State8080 *self)
{
	flagAC(self, self->memory[self->hl], 1);
	self->memory[self->hl]++;
	flagsZSP(self, self->memory[self->hl]);
}

void dcr_m(State8080 *self)
{
	flagAC(self, self->memory[self->hl], -1);
	self->memory[self->hl]--;
	flagsZSP(self, self->memory[self->hl]);
}

void mvi_m_d8(State8080 *self, uint8_t d8)
{
	self->memory[self->hl] = d8;
	self->pc_inc = 2;
}

void stc(State8080 *self)
{
	self->cc.cy = 1;
}

void dad_sp(State8080 *self)
{
	dad(self, self->sp);
}

void lda_adr(State8080 *self, uint8_t low, uint8_t high)
{
	uint16_t address = address_concat(low, high);
	self->a = self->memory[address];

	self->pc_inc = 3;
}

void dcx_sp(State8080 *self)
{
	self->sp--;
}

void inr_a(State8080 *self)
{
	flagAC(self, self->a, 1);
	self->a++;
	flagsZSP(self, self->a);
}

void dcr_a(State8080 *self)
{
	flagAC(self, self->a, -1);
	self->a--;
	flagsZSP(self, self->a);
}

void mvi_a_d8(State8080 *self, uint8_t d8)
{
	self->a = d8;
	self->pc_inc = 2;
}

void cmc(State8080 *self)
{
	self->cc.cy = ~self->cc.cy;
}



/**
 * 0x40 - 0x4F
 */

void mov_b_b(State8080 *self)
{
	// NOP
	// self->b = self->b;
}

void mov_b_c(State8080 *self)
{
	self->b = self->c;
}

void mov_b_d(State8080 *self)
{
	self->b = self->d;
}

void mov_b_e(State8080 *self)
{
	self->b = self->e;
}

void mov_b_h(State8080 *self)
{
	self->b = self->h;
}

void mov_b_l(State8080 *self)
{
	self->b = self->l;
}

void mov_b_m(State8080 *self)
{
	self->b = self->memory[self->hl];
}

void mov_b_a(State8080 *self)
{
	self->b = self->a;
}

void mov_c_b(State8080 *self)
{
	self->c = self->b;
}

void mov_c_c(State8080 *self)
{
	// NOP
	// self->c = self->c;
}

void mov_c_d(State8080 *self)
{
	self->c = self->d;
}

void mov_c_e(State8080 *self)
{
	self->c = self->e;
}

void mov_c_h(State8080 *self)
{
	self->c = self->h;
}

void mov_c_l(State8080 *self)
{
	self->c = self->l;
}

void mov_c_m(State8080 *self)
{
	self->c = self->memory[self->hl];
}

void mov_c_a(State8080 *self)
{
	self->c = self->a;
}



/**
 * 0x50 - 0x5F
 */

void mov_d_b(State8080 *self)
{
	self->d = self->b;
}

void mov_d_c(State8080 *self)
{
	self->d = self->c;
}

void mov_d_d(State8080 *self)
{
	// NOP
	// self->d = self->d;
}

void mov_d_e(State8080 *self)
{
	self->d = self->e;
}

void mov_d_h(State8080 *self)
{
	self->d = self->h;
}

void mov_d_l(State8080 *self)
{
	self->d = self->l;
}

void mov_d_m(State8080 *self)
{
	self->d = self->memory[self->hl];
}

void mov_d_a(State8080 *self)
{
	self->d = self->a;
}

void mov_e_b(State8080 *self)
{
	self->e = self->b;
}

void mov_e_c(State8080 *self)
{
	self->e = self->c;
}

void mov_e_d(State8080 *self)
{
	self->e = self->d;
}

void mov_e_e(State8080 *self)
{
	// NOP
	// self->e = self->e;
}

void mov_e_h(State8080 *self)
{
	self->e = self->h;
}

void mov_e_l(State8080 *self)
{
	self->e = self->l;
}

void mov_e_m(State8080 *self)
{
	self->e = self->memory[self->hl];
}

void mov_e_a(State8080 *self)
{
	self->e = self->a;
}



/**
 * 0x60 - 0x6F
 */

void mov_h_b(State8080 *self)
{
	self->h = self->b;
}

void mov_h_c(State8080 *self)
{
	self->h = self->c;
}

void mov_h_d(State8080 *self)
{
	self->h = self->d;
}

void mov_h_e(State8080 *self)
{
	self->h = self->e;
}

void mov_h_h(State8080 *self)
{
	// NOP
	// self->h = self->h;
}

void mov_h_l(State8080 *self)
{
	self->h = self->l;
}

void mov_h_m(State8080 *self)
{
	self->h = self->memory[self->hl];
}

void mov_h_a(State8080 *self)
{
	self->h = self->a;
}

void mov_l_b(State8080 *self)
{
	self->l = self->b;
}

void mov_l_c(State8080 *self)
{
	self->l = self->c;
}

void mov_l_d(State8080 *self)
{
	self->l = self->d;
}

void mov_l_e(State8080 *self)
{
	self->l = self->e;
}

void mov_l_h(State8080 *self)
{
	self->l = self->h;
}

void mov_l_l(State8080 *self)
{
	// NOP
	// self->l = self->l;
}

void mov_l_m(State8080 *self)
{
	self->l = self->memory[self->hl];
}

void mov_l_a(State8080 *self)
{
	self->l = self->a;
}



/**
 * 0x70 - 0x7F
 */

void mov_m_b(State8080 *self)
{
	self->memory[self->hl] = self->b;
}

void mov_m_c(State8080 *self)
{
	self->memory[self->hl] = self->c;
}

void mov_m_d(State8080 *self)
{
	self->memory[self->hl] = self->d;
}

void mov_m_e(State8080 *self)
{
	self->memory[self->hl] = self->e;
}

void mov_m_h(State8080 *self)
{
	self->memory[self->hl] = self->h;
}

void mov_m_l(State8080 *self)
{
	self->memory[self->hl] = self->l;
}

void hlt(State8080 *self)
{
	// todo
}

void mov_m_a(State8080 *self)
{
	self->memory[self->hl] = self->a;
}

void mov_a_b(State8080 *self)
{
	self->a = self->b;
}

void mov_a_c(State8080 *self)
{
	self->a = self->c;
}

void mov_a_d(State8080 *self)
{
	self->a = self->d;
}

void mov_a_e(State8080 *self)
{
	self->a = self->e;
}

void mov_a_h(State8080 *self)
{
	self->a = self->h;
}

void mov_a_l(State8080 *self)
{
	self->a = self->l;
}

void mov_a_m(State8080 *self)
{
	self->a = self->memory[self->hl];
}

void mov_a_a(State8080 *self)
{
	// NOP
	// self->a = self->a;
}



/**
 * 0x80 - 0x8F
 */


void add_b(State8080 *self)
{
	add(self, self->b);
}

void add_c(State8080 *self)
{
	add(self, self->c);
}

void add_d(State8080 *self)
{
	add(self, self->d);
}

void add_e(State8080 *self)
{
	add(self, self->e);
}

void add_h(State8080 *self)
{
	add(self, self->h);
}

void add_l(State8080 *self)
{
	add(self, self->l);
}

void add_m(State8080 *self)
{
	add(self, self->memory[self->hl]);
}

void add_a(State8080 *self)
{
	add(self, self->a);
}

void adc_b(State8080 *self)
{
	adc(self, self->b);
}

void adc_c(State8080 *self)
{
	adc(self, self->c);
}

void adc_d(State8080 *self)
{
	adc(self, self->d);
}

void adc_e(State8080 *self)
{
	adc(self, self->e);
}

void adc_h(State8080 *self)
{
	adc(self, self->h);
}

void adc_l(State8080 *self)
{
	adc(self, self->l);
}

void adc_m(State8080 *self)
{
	adc(self, self->memory[self->hl]);
}

void adc_a(State8080 *self)
{
	adc(self, self->a);
}



/**
 * 0x90 - 0x9F
 */

// The sub logic *should* work
void sub_b(State8080 *self)
{
	sub(self, self->b);
}

void sub_c(State8080 *self)
{
	sub(self, self->c);
}

void sub_d(State8080 *self)
{
	sub(self, self->d);
}

void sub_e(State8080 *self)
{
	sub(self, self->e);
}

void sub_h(State8080 *self)
{
	sub(self, self->h);
}

void sub_l(State8080 *self)
{
	sub(self, self->l);
}

void sub_m(State8080 *self)
{
	sub(self, self->memory[self->hl]);
}

void sub_a(State8080 *self)
{
	sub(self, self->a);
}

void sbb_b(State8080 *self)
{
	sbb(self, self->b);
}

void sbb_c(State8080 *self)
{
	sbb(self, self->c);
}

void sbb_d(State8080 *self)
{
	sbb(self, self->d);
}

void sbb_e(State8080 *self)
{
	sbb(self, self->e);
}

void sbb_h(State8080 *self)
{
	sbb(self, self->h);
}

void sbb_l(State8080 *self)
{
	sbb(self, self->l);
}

void sbb_m(State8080 *self)
{
	sbb(self, self->memory[self->hl]);
}

void sbb_a(State8080 *self)
{
	sbb(self, self->a);
}



/**
 * 0xA0 - 0xAF
 */

void ana_b(State8080 *self)
{
	ana(self, self->b);
}

void ana_c(State8080 *self)
{
	ana(self, self->c);
}

void ana_d(State8080 *self)
{
	ana(self, self->d);
}

void ana_e(State8080 *self)
{
	ana(self, self->e);
}

void ana_h(State8080 *self)
{
	ana(self, self->h);
}

void ana_l(State8080 *self)
{
	ana(self, self->l);
}

void ana_m(State8080 *self)
{
	ana(self, self->memory[self->hl]);
}

void ana_a(State8080 *self)
{
	ana(self, self->a);
}

void xra_b(State8080 *self)
{
	xra(self, self->b);
}

void xra_c(State8080 *self)
{
	xra(self, self->c);
}

void xra_d(State8080 *self)
{
	xra(self, self->d);
}

void xra_e(State8080 *self)
{
	xra(self, self->e);
}

void xra_h(State8080 *self)
{
	xra(self, self->h);
}

void xra_l(State8080 *self)
{
	xra(self, self->l);
}

void xra_m(State8080 *self)
{
	xra(self, self->memory[self->hl]);
}

void xra_a(State8080 *self)
{
	xra(self, self->a);
}



/**
 * 0xB0 - 0xBF
 */

void ora_b(State8080 *self)
{
	ora(self, self->b);
}

void ora_c(State8080 *self)
{
	ora(self, self->c);
}

void ora_d(State8080 *self)
{
	ora(self, self->d);
}

void ora_e(State8080 *self)
{
	ora(self, self->e);
}

void ora_h(State8080 *self)
{
	ora(self, self->h);
}

void ora_l(State8080 *self)
{
	ora(self, self->l);
}

void ora_m(State8080 *self)
{
	ora(self, self->memory[self->hl]);
}

void ora_a(State8080 *self)
{
	ora(self, self->a);
}

void cmp_b(State8080 *self)
{
	cmp(self, self->b);
}

void cmp_c(State8080 *self)
{
	cmp(self, self->c);
}

void cmp_d(State8080 *self)
{
	cmp(self, self->d);
}

void cmp_e(State8080 *self)
{
	cmp(self, self->e);
}

void cmp_h(State8080 *self)
{
	cmp(self, self->h);
}

void cmp_l(State8080 *self)
{
	cmp(self, self->l);
}

void cmp_m(State8080 *self)
{
	cmp(self, self->memory[self->hl]);
}

void cmp_a(State8080 *self)
{
	cmp(self, self->a);
}




/**
 * 0xC0 - 0xCF
 */

void rnz(State8080 *self)
{
	if (!self->cc.z)
		ret(self);
}

void pop_b(State8080 *self)
{
	pop(self, &self->b, &self->c);
}

void jnz_adr(State8080 *self, uint8_t low, uint8_t high)
{
	jmp_if(self, !self->cc.z, low, high);
}

void jmp_adr(State8080 *self, uint8_t low, uint8_t high)
{
	uint16_t address = address_concat(low, high);

	self->pc = address;
	self->pc_inc = 0;
}

void cnz_adr(State8080 *self, uint8_t low, uint8_t high)
{
	!self->cc.z ? call_adr(self, low, high) : (self->pc_inc = 3);
}

void push_b(State8080 *self)
{
	push(self, self->b, self->c);
}

void adi_d8(State8080 *self, uint8_t d8)
{
	add(self, d8);
	self->pc_inc = 2;
}

void rst_0(State8080 *self)
{
	call_adr(self, 0b000000, 0);
}

void rz(State8080 *self)
{
	if (self->cc.z)
		ret(self);
}

void ret(State8080 *self)
{
	uint8_t pc_low, pc_high;

	pop(self, &pc_high, &pc_low);
	self->pc = address_concat(pc_low, pc_high);

	// To skip over the operands of the inital subroutine call 
	self->pc_inc = 3;
}

void jz_adr(State8080 *self, uint8_t low, uint8_t high)
{
	jmp_if(self, self->cc.z, low, high);
}

void cz_adr(State8080 *self, uint8_t low, uint8_t high)
{
	self->cc.z ? call_adr(self, low, high) : (self->pc_inc = 3);
}

void call_adr(State8080 *self, uint8_t low, uint8_t high)
{
	uint16_t address = address_concat(low, high);
	uint8_t pc_low, pc_high;

// For testing cpu functions
#ifdef CPUDIAG
	if (address == 0x0145) {
		if (self->hl == 0x018b) {
			printf("CPU HAS FAILED. ERROR EXIT=0x%04x\n", self->hl);

			exit(EXIT_FAILURE);
		} else if (self->hl == 0x0174) {
			printf("CPU IS OPERATIONAL\n");

			exit(EXIT_SUCCESS);
		}
	}
#endif

	pc_low = self->pc & 0xff;
	pc_high = (self->pc & 0xff00) >> 8;

	push(self, pc_high, pc_low);

	self->pc = address;
	self->pc_inc = 0;
}

void aci_d8(State8080 *self, uint8_t d8)
{
	adc(self, d8);
	self->pc_inc = 2;
}

void rst_1(State8080 *self)
{
	call_adr(self, 0b001000, 0);
}



/**
 * 0xD0 - 0xDF
 */

void rnc(State8080 *self)
{
	if(!self->cc.cy)
		ret(self);
}

void pop_d(State8080 *self)
{
	pop(self, &self->d, &self->e);
}

void jnc_adr(State8080 *self, uint8_t low, uint8_t high)
{
	jmp_if(self, !self->cc.cy, low, high);
}

void out_d8(State8080 *self, uint8_t d8)
{
	// todo
	// Move data from self->a to device d8
	self->pc_inc = 2;
}

void cnc_adr(State8080 *self, uint8_t low, uint8_t high)
{
	!self->cc.cy ? call_adr(self, low, high) : (self->pc_inc = 3);
}

void push_d(State8080 *self)
{
	push(self, self->d, self->e);
}

void sui_d8(State8080 *self, uint8_t d8)
{
	sub(self, d8);
	self->pc_inc = 2;
}

void rst_2(State8080 *self)
{
	call_adr(self, 0b010000, 0);
}

void rc(State8080 *self)
{
	if (self->cc.cy)
		ret(self);
}

void jc_adr(State8080 *self, uint8_t low, uint8_t high)
{
	jmp_if(self, self->cc.cy, low, high);
}

void in_d8(State8080 *self, uint8_t d8)
{
	// todo
	// Need to read data from input device d8
	self->pc_inc = 2;
}

void cc_adr(State8080 *self, uint8_t low, uint8_t high)
{
	self->cc.cy ? call_adr(self, low, high) : (self->pc_inc = 3);
}

void sbi_d8(State8080 *self, uint8_t d8)
{
	sbb(self, d8);
	self->pc_inc = 2;
}

void rst_3(State8080 *self)
{
	call_adr(self, 0b011000, 0);
}



/**
 * 0xE0 - 0xEF
 */

void rpo(State8080 *self)
{
	if (!self->cc.p)
		ret(self);
}

void pop_h(State8080 *self)
{
	pop(self, &self->h, &self->l);
}

void jpo_adr(State8080 *self, uint8_t low, uint8_t high)
{
	jmp_if(self, !self->cc.p, low, high);
}

void xthl(State8080 *self)
{
	// Exchanges l
	uint8_t temp = self->l;
	self->l = self->memory[self->sp];
	self->memory[self->sp] = temp;

	// Exchanges h
	temp = self->h;
	self->h = self->memory[self->sp + 1];
	self->memory[self->sp + 1] = temp;
}

void cpo_adr(State8080 *self, uint8_t low, uint8_t high)
{
	!self->cc.z ? call_adr(self, low, high) : (self->pc_inc = 3);
}

void push_h(State8080 *self)
{
	push(self, self->h, self->l);
}

void ani_d8(State8080 *self, uint8_t d8)
{
	ana(self, d8);
	self->pc_inc = 2;
}

void rst_4(State8080 *self)
{
	call_adr(self, 0b100000, 0);
}

void rpe(State8080 *self)
{
	if (self->cc.p)
		ret(self);
}

void pchl(State8080 *self)
{
	self->pc = self->hl;
	self->pc_inc = 0;
}

void jpe_adr(State8080 *self, uint8_t low, uint8_t high)
{
	jmp_if(self, self->cc.p, low, high);
}

void xchg(State8080 *self)
{
	uint16_t temp = self->hl;
	self->hl = self->de;
	self->de = temp;
}

void cpe_adr(State8080 *self, uint8_t low, uint8_t high)
{
	self->cc.p ? call_adr(self, low, high) : (self->pc_inc = 3);
}

void xri_d8(State8080 *self, uint8_t d8)
{
	// AC isn't affected by xri, but xra does.
	// This is done to undo it.
	uint8_t temp = self->cc.ac;
	xra(self, d8);
	self->cc.ac = temp;

	self->pc_inc = 2;
}

void rst_5(State8080 *self)
{
	call_adr(self, 0b101000, 0);
}



/**
 * 0xF0 - 0xFF
 */

void rp(State8080 *self)
{
	if (!self->cc.s)
		ret(self);
}

void pop_psw(State8080 *self)
{
	uint8_t *status = (uint8_t *) &self->cc;
	pop(self, &self->a, status);

	// Debugging
	// If this function is working properly, none of the BIT variables
	// should be different from their original values
	if (!self->cc.BIT_1 || self->cc.BIT_3 || self->cc.BIT_5) {
		printf("ERROR: Condition bit byte has impossible value\n");
		printf("Carry:  %x\n", self->cc.cy);
		printf("BIT_1:  %x\n", self->cc.BIT_1);
		printf("Parity: %x\n", self->cc.p);
		printf("BIT_3:  %x\n", self->cc.BIT_3);
		printf("AC:     %x\n", self->cc.ac);
		printf("BIT_5:  %x\n", self->cc.BIT_5);
		printf("Zero:   %x\n", self->cc.z);
		printf("Sign:   %x\n", self->cc.s);

		exit(EXIT_FAILURE);
	}
}

void jp_adr(State8080 *self, uint8_t low, uint8_t high)
{
	jmp_if(self, !self->cc.s, low, high);
}

void di(State8080 *self)
{
	self->int_enable = 0;
}

void cp_adr(State8080 *self, uint8_t low, uint8_t high)
{
	!self->cc.s ? call_adr(self, low, high) : (self->pc_inc = 3);
}

void push_psw(State8080 *self)
{
	// This is sus. I did this to prevent a warning.
	// self->cc should be the value of the settings of the condition bits
	uint8_t status = *(uint8_t *) &self->cc;
	push(self, self->a, status);
}

void ori_d8(State8080 *self, uint8_t d8)
{
	ora(self, d8);
	self->pc_inc = 2;
}

void rst_6(State8080 *self)
{
	call_adr(self, 0b110000, 0);
}

void rm(State8080 *self)
{
	if(self->cc.s)
		ret(self);
}

void sphl(State8080 *self)
{
	self->sp = self->hl;
}

void jm_adr(State8080 *self, uint8_t low, uint8_t high)
{
	jmp_if(self, self->cc.s, low, high);
}

void ei(State8080 *self)
{
	self->int_enable = 1;
}

void cm_adr(State8080 *self, uint8_t low, uint8_t high)
{
	self->cc.s ? call_adr(self, low, high) : (self->pc_inc = 3);
}

void cpi_d8(State8080 *self, uint8_t d8)
{
	cmp(self, d8);
	self->pc_inc = 2;
}

void rst_7(State8080 *self)
{
	call_adr(self, 0b111000, 0);
}
