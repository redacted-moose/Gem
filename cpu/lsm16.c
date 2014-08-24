/*
 * lsm16.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../gem.h"
#include "../cpu.h"
#include "../mmu.h"
#include "lsm16.h"


void LD_BC_d16()
{
	cpu.bc = read_word(cpu.pc);
	cpu.pc += 2;
	cpu.t += 12;
	cpu.last_t = 12;
}

void LD_a16_SP()
{
	write_word(read_word(cpu.pc), cpu.sp);
	cpu.pc++;
	cpu.t += 20;
	cpu.last_t = 20;
}

void LD_DE_d16()
{
	cpu.de = read_word(cpu.pc);
	cpu.pc += 2;
	cpu.t += 12;
	cpu.last_t = 12;
}

void LD_HL_d16()
{
	cpu.hl = read_word(cpu.pc);
	cpu.pc += 2;
	cpu.t += 12;
	cpu.last_t = 12;
}

void LD_SP_d16()
{
	cpu.sp = read_word(cpu.pc);
	cpu.pc += 2;
	cpu.t += 12;
	cpu.last_t = 12;
}

void POP_BC()
{
	cpu.bc = read_word(cpu.sp);
	cpu.sp += 2;
	cpu.t += 12;
	cpu.last_t = 12;
}

void PUSH_BC()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.bc);
	cpu.t += 16;
	cpu.last_t = 16;
}

void POP_DE()
{
	cpu.de = read_word(cpu.sp);
	cpu.sp += 2;
	cpu.t += 12;
	cpu.last_t = 12;
}

void PUSH_DE()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.de);
	cpu.t += 16;
	cpu.last_t = 16;
}

void POP_HL()
{
	cpu.hl = read_word(cpu.sp);
	cpu.sp += 2;
	cpu.t += 12;
	cpu.last_t = 12;
}

void PUSH_HL()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.hl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void POP_AF()
{
	cpu.af = read_word(cpu.sp);
	cpu.sp += 2;
	cpu.t += 12;
	cpu.last_t = 12;
}

void PUSH_AF()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.af);
	cpu.t += 16;
	cpu.last_t = 16;
}

void LD_HL_SP_r8()
{
	cpu.hl = read_word(cpu.sp + (s_byte)read_byte(cpu.pc));
	cpu.pc++;
	cpu.t += 12;
	cpu.last_t = 12;
}

void LD_SP_HL()
{
	cpu.sp = cpu.hl;
	cpu.t += 8;
	cpu.last_t = 8;
}
