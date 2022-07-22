#include "../include/8080emu.h"

#include <unistd.h>
#include <sys/ioctl.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

static void print_debug(State8080 *self)
{
	const int X_OFFSET = 11;
	const int Y_OFFSET = 1;

	static unsigned short rows, cols;
	static int line_count = 1;
	struct winsize window;
	uint8_t psw = *(uint8_t *)&self->cc;


	ioctl(STDIN_FILENO, TIOCGWINSZ, &window);
	
	if (rows != window.ws_row || cols != window.ws_col || line_count % (rows - 1) == 0) {
		system("clear");
		line_count = 1;
	}

	rows = window.ws_row;
	cols = window.ws_col;


	gotoxy(cols - X_OFFSET, 1 + Y_OFFSET);
	printf("pc=0x%04x", self->pc);

	gotoxy(cols - X_OFFSET, 2 + Y_OFFSET);
	printf("sp=0x%04x", self->sp);

	gotoxy(cols - X_OFFSET, 4 + Y_OFFSET);
	printf("bc=0x%04x", self->bc);
	gotoxy(cols - X_OFFSET, 5 + Y_OFFSET);
	printf("de=0x%04x", self->de);
	gotoxy(cols - X_OFFSET, 6 + Y_OFFSET);
	printf("hl=0x%04x", self->hl);
	gotoxy(cols - X_OFFSET, 7 + Y_OFFSET);
	printf("a=0x%02x", self->a);
	gotoxy(cols - X_OFFSET, 8 + Y_OFFSET);
	printf("psw=0x%02x", psw);

	gotoxy(cols - X_OFFSET, 10 + Y_OFFSET);
	printf("cy=%d p=%d", self->cc.cy, self->cc.p);
	gotoxy(cols - X_OFFSET, 11 + Y_OFFSET);
	printf("ac=%d z=%d", self->cc.ac, self->cc.z);
	gotoxy(cols - X_OFFSET, 12 + Y_OFFSET);
	printf("s=%d", self->cc.s);


	gotoxy(0, line_count % (rows - 1));
	line_count++;
}

// Emulates an 8080 instruction based on current PC
void emulate8080Op(State8080 *self)
{
	uint8_t *opcode = &self->memory[self->pc];

	disassemble8080Op(self->memory, self->pc);

	switch (*opcode)
	{
		case 0x00: break;		// NOP
		case 0x01: 
			lxi_b_d16(self, opcode[1], opcode[2]); 
			break;
		case 0x02:
			stax_b(self);
			break;
		case 0x03:
			inx_b(self);
			break;
		case 0x04:
			inr_b(self);
			break;
		case 0x05:
			dcr_b(self);
			break;
		case 0x06:
			mvi_b_d8(self, opcode[1]);
			break;
		case 0x07:		
			rlc(self);
			break;
		case 0x08: break;	// Blank instruction
		case 0x09:
			dad_b(self);
			break;
		case 0x0a:
			ldax_b(self);
			break;
		case 0x0b:
			dcx_b(self);
			break;
		case 0x0c:
			inr_c(self);
			break;
		case 0x0d:
			dcr_c(self);
			break;
		case 0x0e:
			mvi_c_d8(self, opcode[1]);
			break;
		case 0x0f:
			rrc(self);
			break;
		case 0x10: break;	// Blank instruction
		case 0x11:
			lxi_d_d16(self, opcode[1], opcode[2]);
			break;
		case 0x12:
			stax_d(self);
			break;
		case 0x13:
			inx_d(self);
			break;
		case 0x14:
			inr_d(self);
			break;
		case 0x15:
			dcr_d(self);
			break;
		case 0x16:
			mvi_d_d8(self, opcode[1]);
			break;
		case 0x17:
			ral(self);
			break;
		case 0x18: break;	// Blank Instruction
		case 0x19:
			dad_d(self);
			break;
		case 0x1a:
			ldax_d(self);
			break;
		case 0x1b:
			dcx_d(self);
			break;
		case 0x1c:
			inr_e(self);
			break;
		case 0x1d:
			dcr_e(self);
			break;
		case 0x1e:
			mvi_e_d8(self, opcode[1]);
			break;
		case 0x1f:
			rar(self);
			break;
		case 0x20: break;	// Blank Instruction
		case 0x21:
			lxi_h_d16(self, opcode[1], opcode[2]);
			break;
		case 0x22:
 			shld_adr(self, opcode[1], opcode[2]);
			break;
		case 0x23:
			inx_h(self);
			break;
		case 0x24:
			inr_h(self);
			break;
		case 0x25:
			dcr_h(self);
			break;
		case 0x26:
			mvi_h_d8(self, opcode[1]);
			break;
		case 0x27:
			daa(self);
			break;
		case 0x28: break;	// Blank Instruction
		case 0x29:
			dad_h(self);
			break;
		case 0x2a:
			lhld_adr(self, opcode[1], opcode[2]);
			break;
		case 0x2b:
			dcx_h(self);
			break;
		case 0x2c:
			inr_l(self);
			break;
		case 0x2d:
			dcr_l(self);
			break;
		case 0x2e:
			mvi_l_d8(self, opcode[1]);
			break;
		case 0x2f:			// CMA
			cma(self);
			break;
		case 0x30: break;	// Blank Instruction
		case 0x31:
			lxi_sp_d16(self, opcode[1], opcode[2]);
			break;
		case 0x32:
			sta_adr(self, opcode[1], opcode[2]);
			break;
		case 0x33:
			inx_sp(self);
			break;
		case 0x34:
			inr_m(self);
			break;
		case 0x35:
			dcr_m(self);
			break;
		case 0x36:
			mvi_m_d8(self, opcode[1]);
			break;
		case 0x37:
			stc(self);
			break;
		case 0x38: break;	// Blank Instruction
		case 0x39:
			dad_sp(self);
			break;
		case 0x3a:
			lda_adr(self, opcode[1], opcode[2]);
			break;
		case 0x3b:
			dcx_sp(self);
			break;
		case 0x3c:
			inr_a(self);
			break;
		case 0x3d:
			dcr_a(self);
			break;
		case 0x3e:
			mvi_a_d8(self, opcode[1]);
			break;
		case 0x3f:
			cmc(self);
			break;
		case 0x40:
			mov_b_b(self);
			break;
		case 0x41:
			mov_b_c(self);
			break;
		case 0x42:
			mov_b_d(self);
			break;
		case 0x43:
			mov_b_e(self);
			break;
		case 0x44:
			mov_b_h(self);
			break;
		case 0x45:
			mov_b_l(self);
			break;
		case 0x46:
			mov_b_m(self);
			break;
		case 0x47:
			mov_b_a(self);
			break;
		case 0x48:
			mov_c_b(self);
			break;
		case 0x49:
			mov_c_c(self);
			break;
		case 0x4a:
			mov_c_d(self);
			break;
		case 0x4b:
			mov_c_e(self);
			break;
		case 0x4c:
			mov_c_h(self);
			break;
		case 0x4d:
			mov_c_l(self);
			break;
		case 0x4e:
			mov_c_m(self);
			break;
		case 0x4f:
			mov_c_a(self);
			break;
		case 0x50:
			mov_d_b(self);
			break;
		case 0x51:
			mov_d_c(self);
			break;
		case 0x52:
			mov_d_d(self);
			break;
		case 0x53:
			mov_d_e(self);
			break;
		case 0x54:
			mov_d_h(self);
			break;
		case 0x55:
			mov_d_l(self);
			break;
		case 0x56:
			mov_d_m(self);
			break;
		case 0x57:
			mov_d_a(self);
			break;
		case 0x58:
			mov_e_b(self);
			break;
		case 0x59:
			mov_e_c(self);
			break;
		case 0x5a:
			mov_e_d(self);
			break;
		case 0x5b:
			mov_e_e(self);
			break;
		case 0x5c:
			mov_e_h(self);
			break;
		case 0x5d:
			mov_e_l(self);
			break;
		case 0x5e:
			mov_e_m(self);
			break;
		case 0x5f:
			mov_e_a(self);
			break;
		case 0x60:
			mov_h_b(self);
			break;
		case 0x61:
			mov_h_c(self);
			break;
		case 0x62:
			mov_h_d(self);
			break;
		case 0x63:
			mov_h_e(self);
			break;
		case 0x64:
			mov_h_h(self);
			break;
		case 0x65:
			mov_h_l(self);
			break;
		case 0x66:
			mov_h_m(self);
			break;
		case 0x67:
			mov_h_a(self);
			break;
		case 0x68:
			mov_l_b(self);
			break;
		case 0x69:
			mov_l_c(self);
			break;
		case 0x6a:
			mov_l_d(self);
			break;
		case 0x6b:
			mov_l_e(self);
			break;
		case 0x6c:
			mov_l_h(self);
			break;
		case 0x6d:
			mov_l_l(self);
			break;
		case 0x6e:
			mov_l_m(self);
			break;
		case 0x6f:
			mov_l_a(self);
			break;
		case 0x70:
			mov_m_b(self);
			break;
		case 0x71:
			mov_m_c(self);
			break;
		case 0x72:
			mov_m_d(self);
			break;
		case 0x73:
			mov_m_e(self);
			break;
		case 0x74:
			mov_m_h(self);
			break;
		case 0x75:
			mov_m_l(self);
			break;
		case 0x76:
			hlt(self);
			break;
		case 0x77:
			mov_m_a(self);
			break;
		case 0x78:
			mov_a_b(self);
			break;
		case 0x79:
			mov_a_c(self);
			break;
		case 0x7a:
			mov_a_d(self);
			break;
		case 0x7b:
			mov_a_e(self);
			break;
		case 0x7c:
			mov_a_h(self);
			break;
		case 0x7d:
			mov_a_l(self);
			break;
		case 0x7e:
			mov_a_m(self);
			break;
		case 0x7f:
			mov_a_a(self);
			break;
		case 0x80:
			add_b(self);
			break;
		case 0x81:
			add_c(self);
			break;
		case 0x82:
			add_d(self);
			break;
		case 0x83:
			add_e(self);
			break;
		case 0x84:
			add_h(self);
			break;
		case 0x85:
			add_l(self);
			break;
		case 0x86:
			add_m(self);
			break;
		case 0x87:
			add_a(self);
			break;
		case 0x88:
			adc_b(self);
			break;
		case 0x89:
			adc_c(self);
			break;
		case 0x8a:
			adc_d(self);
			break;
		case 0x8b:
			adc_e(self);
			break;
		case 0x8c:
			adc_h(self);
			break;
		case 0x8d:
			adc_l(self);
			break;
		case 0x8e:
			adc_m(self);
			break;
		case 0x8f:
			adc_a(self);
			break;
		case 0x90:
			sub_b(self);
			break;
		case 0x91:
			sub_c(self);
			break;
		case 0x92:
			sub_d(self);
			break;
		case 0x93:
			sub_e(self);
			break;
		case 0x94:
			sub_h(self);
			break;
		case 0x95:
			sub_l(self);
			break;
		case 0x96:
			sub_m(self);
			break;
		case 0x97:
			sub_a(self);
			break;
		case 0x98:
			sbb_b(self);
			break;
		case 0x99:
			sbb_c(self);
			break;
		case 0x9a:
			sbb_d(self);
			break;
		case 0x9b:
			sbb_e(self);
			break;
		case 0x9c:
			sbb_h(self);
			break;
		case 0x9d:
			sbb_l(self);
			break;
		case 0x9e:
			sbb_m(self);
			break;
		case 0x9f:
			sbb_a(self);
			break;
		case 0xa0:
			ana_b(self);
			break;
		case 0xa1:
			ana_c(self);
			break;
		case 0xa2:
			ana_d(self);
			break;
		case 0xa3:
			ana_e(self);
			break;
		case 0xa4:
			ana_h(self);
			break;
		case 0xa5:
			ana_l(self);
			break;
		case 0xa6:
			ana_m(self);
			break;
		case 0xa7:
			ana_a(self);
			break;
		case 0xa8:
			xra_b(self);
			break;
		case 0xa9:
			xra_c(self);
			break;
		case 0xaa:
			xra_d(self);
			break;
		case 0xab:
			xra_e(self);
			break;
		case 0xac:
			xra_h(self);
			break;
		case 0xad:
			xra_l(self);
			break;
		case 0xae:
			xra_m(self);
			break;
		case 0xaf:
			xra_a(self);
			break;
		case 0xb0:
			ora_b(self);
			break;
		case 0xb1:
			ora_c(self);
			break;
		case 0xb2:
			ora_d(self);
			break;
		case 0xb3:
			ora_e(self);
			break;
		case 0xb4:
			ora_h(self);
			break;
		case 0xb5:
			ora_l(self);
			break;
		case 0xb6:
			ora_m(self);
			break;
		case 0xb7:
			ora_a(self);
			break;
		case 0xb8:
			cmp_b(self);
			break;
		case 0xb9:
			cmp_c(self);
			break;
		case 0xba:
			cmp_d(self);
			break;
		case 0xbb:
			cmp_e(self);
			break;
		case 0xbc:
			cmp_h(self);
			break;
		case 0xbd:
			cmp_l(self);
			break;
		case 0xbe:
			cmp_m(self);
			break;
		case 0xbf:
			cmp_a(self);
			break;
		case 0xc0:
			rnz(self);
			break;
		case 0xc1:
			pop_b(self);
			break;
		case 0xc2:
			jnz_adr(self, opcode[1], opcode[2]);
			break;
		case 0xc3:
			jmp_adr(self, opcode[1], opcode[2]);
			break;
		case 0xc4:
			cnz_adr(self, opcode[1], opcode[2]);
			break;
		case 0xc5:
			push_b(self);
			break;
		case 0xc6:
			adi_d8(self, opcode[1]);
			break;
		case 0xc7:
			rst_0(self);
			break;
		case 0xc8:
			rz(self);
			break;
		case 0xc9:
			ret(self);
			break;
		case 0xca:
			jz_adr(self, opcode[1], opcode[2]);
			break;
		case 0xcb: break;	// Blank Instruction
		case 0xcc:
			cz_adr(self, opcode[1], opcode[2]);
			break;
		case 0xcd:
			call_adr(self, opcode[1], opcode[2]);
			break;
		case 0xce:
			aci_d8(self, opcode[1]);
			break;
		case 0xcf:
			rst_1(self);
			break;
		case 0xd0:
			rnc(self);
			break;
		case 0xd1:
			pop_d(self);
			break;
		case 0xd2:
			jnc_adr(self, opcode[1], opcode[2]);
			break;
		case 0xd3:
			out_d8(self, opcode[1]);
			break;
		case 0xd4:
			cnc_adr(self, opcode[1], opcode[2]);
			break;
		case 0xd5:
			push_d(self);
			break;
		case 0xd6:
			sui_d8(self, opcode[1]);
			break;
		case 0xd7:
			rst_2(self);
			break;
		case 0xd8:
			rc(self);
			break;
		case 0xd9: break;	// Blank Instruction
		case 0xda:
			jc_adr(self, opcode[1], opcode[2]);
			break;
		case 0xdb:
			in_d8(self, opcode[1]);
			break;
		case 0xdc:
			cc_adr(self, opcode[1], opcode[2]);
			break;
		case 0xdd: break;	// Blank Instruction
		case 0xde:
			sbi_d8(self, opcode[1]);
			break;
		case 0xdf:
			rst_3(self);
			break;
		case 0xe0:
			rpo(self);
			break;
		case 0xe1:
			pop_h(self);
			break;
		case 0xe2:
			jpo_adr(self, opcode[1], opcode[2]);
			break;
		case 0xe3:
			xthl(self);
			break;
		case 0xe4:
			cpo_adr(self, opcode[1], opcode[2]);
			break;
		case 0xe5:
			push_h(self);
			break;
		case 0xe6:
			ani_d8(self, opcode[1]);
			break;
		case 0xe7:
			rst_4(self);
			break;
		case 0xe8:
			rpe(self);
			break;
		case 0xe9:
			pchl(self);
			break;
		case 0xea:
			jpe_adr(self, opcode[1], opcode[2]);
			break;
		case 0xeb:
			xchg(self);
			break;
		case 0xec:
			cpe_adr(self, opcode[1], opcode[2]);
			break;
		case 0xed: break;	// Blank Instruction
		case 0xee:
			xri_d8(self, opcode[1]);
			break;
		case 0xef:
			rst_5(self);
			break;
		case 0xf0:
			rp(self);
			break;
		case 0xf1:
			pop_psw(self);
			break;
		case 0xf2:
			jp_adr(self, opcode[1], opcode[2]);
			break;
		case 0xf3:
			di(self);
			break;
		case 0xf4:
			cp_adr(self, opcode[1], opcode[2]);
			break;
		case 0xf5:
			push_psw(self);
			break;
		case 0xf6:
			ori_d8(self, opcode[1]);
			break;
		case 0xf7:
			rst_6(self);
			break;
		case 0xf8:
			rm(self);
			break;
		case 0xf9:
			sphl(self);
			break;
		case 0xfa:
			jm_adr(self, opcode[1], opcode[2]);
			break;
		case 0xfb:
			ei(self);
			break;
		case 0xfc:
			cm_adr(self, opcode[1], opcode[2]);
			break;
		case 0xfd: break;	// Blank Instruction
		case 0xfe:
			cpi_d8(self, opcode[1]);
			break;
		case 0xff:
			rst_7(self);
			break;

		default: unimplementedInstruction(*opcode);
	}

	print_debug(self);

	self->pc += self->pc_inc;
	self->pc_inc = 1;
}