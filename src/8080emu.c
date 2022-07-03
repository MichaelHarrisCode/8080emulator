#include "../include/8080emu.h"
// Emulates an 8080 instruction based on current PC
int emulate8080Op(State8080 *self)
{
	uint8_t *opcode = &self->memory[self->pc];

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
			
			break;

		default: unimplementedInstruction(*opcode);
	}
	self->pc++;
}