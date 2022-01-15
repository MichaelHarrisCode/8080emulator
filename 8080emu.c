#include "8080emu.h"

// Emulates an 8080 instruction based on current PC
int emulate8080Op(State8080 *state)
{
	uint8_t *opcode = &state->memory[state->pc];

	switch (*opcode)
	{
		case 0x00: break;	// NOP
		case 0x01:			// LXI	B,D16
			state->c = opcode[1];
			state->b = opcode[2];
			state->pc += 2;
			break;
		case 0x02:			// STAX	B
			state->memory[state->bc] = state->a;
			break;
		case 0x03:			// INX B
			state->bc++;
			break;
		case 0x04:			// INR B
			state->b++;
			flagsZSP(state, state->b);
			flagAC(state, state->b, 1);
			break;
		case 0x05:			// DCR B
			state->b--;
			flagsZSP(state, state->b);
			flagAC(state, state->b, -1);
			break;
		case 0x06:			// MVI B,D8
			state->b = opcode[1];
			state->pc++;
			break;
		case 0x07:			// RLC			Not sure if this works right. Probably test the logic.
			{
				uint8_t bit_7 = (0x80 == (state->a & 0x80));
				state->a <<= 1;
				state->a = state->a | bit_7;
				state->cc.cy = bit_7;
			}
			break;
		case 0x08: break;	// Blank instruction
		case 0x09:			// DAD B
			state->hl += state->bc;

			state->cc.cy = ((uint32_t)(state->hl + state->bc) > 0xffff);
			break;
		case 0x0a:			// LDAX B
			state->a = state->memory[state->bc];
			break;
		case 0x0b:			// DCX B
			state->bc--;
			break;
		case 0x0c:			// INR C
			state->c++;
			flagsZSP(state, state->c);
			flagAC(state, state->c, 1);
			break;
		case 0x0d:			// DCR C
			state->c--;
			flagsZSP(state, state->c);
			flagAC(state, state->c, -1);
			break;
		case 0x0e:			// MVI C,D8
			state->c = opcode[1];
			state->pc++;
			break;
		case 0x0f:			// RRC			Think this through again
			{
				uint8_t bit_0 = (state->a & 0x1);
				state->a >>= 1;
				state->cc.cy = bit_0;
				bit_0 <<= 7;
				state->a = state->a | bit_0;
			}
			break;
		case 0x10: break;	// Blank instruction
		case 0x11:			// LXI D,D16 
			state->e = opcode[1];
			state->d = opcode[2];
			state->pc += 2;
			break;
		case 0x12:			// STAX D
			state->memory[state->de] = state->a;
			break;
		case 0x13:			// INX D
			state->de++;
			break;
		case 0x14:			// INR D
			state->d++;
			flagsZSP(state, state->d);
			flagAC(state, state->d, 1);
			break;
		case 0x15:			// DCR D
			state->d--;
			flagsZSP(state, state->d);
			flagAC(state, state->d, -1);
			break;
		case 0x16:			// MVI D,D8
			state->d = opcode[1];
			state->pc++;
			break;
		case 0x17:			// RAL
			{
				uint8_t bit_7 = (0x80 == (0x80 & state->a));
				state->a <<= 1;
				state->a = state->a | state->cc.cy;
				state->cc.cy = bit_7;
			}
			break;
		case 0x18: break;	// Blank Instruction
		case 0x19:			// DAD D
			state->hl = state->hl + state->de;

			state->cc.cy = ((uint32_t)(state->hl + state->de) > 0xffff);
			break;
		case 0x1a:			// LDAX D
			state->a = state->memory[state->de];
			break;
		case 0x1b:			// DCX D
			state->de--;
			break;
		case 0x1c:			// INR E
			state->e++;
			flagsZSP(state, state->e);
			flagAC(state, state->e, 1);
			break;
		case 0x1d:			// DCR E
			state->e--;
			flagsZSP(state, state->e);
			flagAC(state, state->e, -1);
			break;
		case 0x1e:			// MVI E,D8
			state->e = opcode[1];
			state->pc++;
			break;
		case 0x1f:			// RAR			Did I get this right?
			{
				uint8_t bit_0 = (state->a & 0x1);
				state->a >>= 1;
				state->a = state->a | (state->cc.cy << 7);
				state->cc.cy = bit_0;
			}
			break;
		case 0x20: break;	// Blank Instruction
		case 0x21:			// LXI H,D16
			state->l = opcode[1];
			state->h = opcode[2];
			state->pc += 2;
			break;
		case 0x22:			// SHLD adr     Is this correct?
 			{
				uint16_t address = opcode[1] | (opcode[2] << 8);
				state->memory[address] = state->l;
				state->memory[address + 1] = state->h;
				state->pc += 2;
			}
			break;
		case 0x23:			// INX H
			state->hl++;
			break;
		case 0x24:			// INR H
			state->h++;
			flagsZSP(state, state->h);
			flagAC(state, state->h, 1);
			break;
		case 0x25:			// DCR H
			state->h--;
			flagsZSP(state, state->h);
			flagAC(state, state->h, -1);
			break;
		case 0x26:			// MVI H,D8
			state->h = opcode[1];
			state->pc++;
			break;
		case 0x27:			// DAA
			// WHAT THE FRIIIIIIICK DO I DO HERE?
			break;
		case 0x28: break;	// Blank Instruction
		case 0x29:			// DAD H
			state->cc.cy = (0x80 == (0x80 & state->hl));
			// Apparently, adding two 16-bit numbers to itself is equivalent to 
			// shifting it to the left by one bit. 
			// Original should be: state->hl = state->hl + state->hl;
			state->h <<= 1;
			break;
		case 0x2a:			// LHLD adr
			state->l = opcode[1];
			state->h = opcode[2];
			state->pc += 2;
			break;
		case 0x2b:			// DCX H
			state->hl--;
			break;
		case 0x2c:			// INR L
			state->l++;
			flagsZSP(state, state->l);
			flagAC(state, state->l, 1);
			break;
		case 0x2d:			// DCR L
			state->l--;
			flagsZSP(state, state->l);
			flagAC(state, state->l, -1);
			break;
		case 0x2e:			// MVI L,D8
			state->l = opcode[1];
			state->pc++;
			break;
		case 0x2f:			// CMA
			state->a = ~state->a;
			break;
		case 0x30: break;	// Blank Instruction
		case 0x31:			// LXI SP,D16
			
			break;

		default: unimplementedInstruction(*opcode);
	}
	state->pc++;
}