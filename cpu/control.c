/*
 * control.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../gem.h"
#include "../cpu.h"
#include "../mmu.h"
#include "control.h"

void JR_r8()
{
	cpu.pc += (s_byte)read_byte(cpu.pc);
	cpu.pc++;
	cpu.t += 12;
	cpu.last_t = 12;
}

void JR_NZ_r8()
{
	if(!cpu.z) {
		cpu.pc += (s_byte)read_byte(cpu.pc);
		cpu.pc++;
		cpu.t += 12;
		cpu.last_t = 12;
		return;
	} else {
		cpu.pc++;
		cpu.t += 8;
		cpu.last_t = 8;
	}
}

void JR_Z_r8()
{
	if(cpu.z) {
		cpu.pc += (s_byte)read_byte(cpu.pc);
		cpu.pc++;
		cpu.t += 12;
		cpu.last_t = 12;
		return;
	} else {
		cpu.pc++;
		cpu.t += 8;
		cpu.last_t = 8;
	}
}

void JR_NC_r8()
{
	if(!cpu.ca) {
		cpu.pc += (s_byte)read_byte(cpu.pc);
		cpu.pc++;
		cpu.t += 12;
		cpu.last_t = 12;
		return;
	} else {
		cpu.pc++;
		cpu.t += 8;
		cpu.last_t = 8;
	}
}

void JR_C_r8()
{
	if(cpu.ca) {
		cpu.pc += (s_byte)read_byte(cpu.pc);
		cpu.pc++;
		cpu.t += 12;
		cpu.last_t = 12;
		return;
	} else {
		cpu.pc++;
		cpu.t += 8;
		cpu.last_t = 8;
	}
}

void RET_NZ()
{
	if(!cpu.z) {
		cpu.pc = read_word(cpu.sp);
		cpu.sp += 2;
		cpu.t += 20;
		cpu.last_t = 20;
	} else {
		cpu.pc += 2;
		cpu.t += 8;
		cpu.last_t = 8;
	}
}

void JP_NZ_a16()
{
	if(!cpu.z) {
		cpu.pc = read_word(cpu.pc);
		cpu.t += 16;
		cpu.last_t = 16;
		return;
	} else {
		cpu.pc += 2;
		cpu.t += 12;
		cpu.last_t = 12;
	}
}

void JP_a16()
{
	cpu.pc = read_word(cpu.pc);
	cpu.t += 16;
	cpu.last_t = 16;
}

void CALL_NZ_a16()
{
	if(!cpu.z) {
		cpu.sp -= 2;
		write_word(cpu.sp, cpu.pc + 2);
		cpu.pc = read_word(cpu.pc);
		cpu.t += 24;
		cpu.last_t = 24;
	} else {
		cpu.pc += 2;
		cpu.t += 12;
		cpu.last_t = 12;
	}
}

void RST_00H()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc);
	cpu.pc = 0x0000;
	cpu.t += 16;
	cpu.last_t = 16;
}

void RET_Z()
{
	if(cpu.z) {
		cpu.pc = read_word(cpu.sp);
		cpu.sp += 2;
		cpu.t += 20;
		cpu.last_t = 20;
	} else {
		cpu.pc += 2;
		cpu.t += 8;
		cpu.last_t = 8;
	}
}

void RET()
{
	cpu.pc = read_word(cpu.sp);
	cpu.sp += 2;
	cpu.t += 16;
	cpu.last_t = 16;
}

void JP_Z_a16()
{
	if(cpu.z) {
		cpu.pc = read_word(cpu.pc);
		cpu.t += 16;
		cpu.last_t = 16;
		return;
	} else {
		cpu.pc += 2;
		cpu.t += 12;
		cpu.last_t = 12;
	}
}

void CALL_Z_a16()
{
	if(cpu.z) {
		cpu.sp -= 2;
		write_word(cpu.sp, cpu.pc + 2);
		cpu.pc = read_word(cpu.pc);
		cpu.t += 24;
		cpu.last_t = 24;
	} else {
		cpu.pc += 2;
		cpu.t += 12;
		cpu.last_t = 12;
	}
}

void CALL_a16()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc + 2);
	cpu.pc = read_word(cpu.pc);
	cpu.t += 24;
	cpu.last_t = 24;
}

void RST_08H()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc);
	cpu.pc = 0x0008;
	cpu.t += 16;
	cpu.last_t = 16;
}

void RET_NC()
{
	if(!cpu.ca) {
		cpu.pc = read_word(cpu.sp);
		cpu.sp += 2;
		cpu.t += 20;
		cpu.last_t = 20;
	} else {
		cpu.pc += 2;
		cpu.t += 8;
		cpu.last_t = 8;
	}
}

void JP_NC_a16()
{
	if(!cpu.ca) {
		cpu.pc = read_word(cpu.pc);
		cpu.t += 16;
		cpu.last_t = 16;
		return;
	} else {
		cpu.pc += 2;
		cpu.t += 12;
		cpu.last_t = 12;
	}
}

void CALL_NC_a16()
{
	if(!cpu.c) {
		cpu.sp -= 2;
		write_word(cpu.sp, cpu.pc + 2);
		cpu.pc = read_word(cpu.pc);
		cpu.t += 24;
		cpu.last_t = 24;
	} else {
		cpu.pc += 2;
		cpu.t += 12;
		cpu.last_t = 12;
	}
}

void RST_10H()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc);
	cpu.pc = 0x0010;
	cpu.t += 16;
	cpu.last_t = 16;
}

void RET_C()
{
	if(cpu.ca) {
		cpu.pc = read_word(cpu.sp);
		cpu.sp += 2;
		cpu.t += 20;
		cpu.last_t = 20;
	} else {
		cpu.pc += 2;
		cpu.t += 8;
		cpu.last_t = 8;
	}
}

void RETI()
{
	cpu.pc = read_word(cpu.sp);
	cpu.sp += 2;
	cpu.ime = true;
	cpu.t += 16;
	cpu.last_t = 16;
}

void JP_C_a16()
{
	if(cpu.ca) {
		cpu.pc = read_word(cpu.pc);
		cpu.t += 16;
		cpu.last_t = 16;
		return;
	} else {
		cpu.pc += 2;
		cpu.t += 12;
		cpu.last_t = 12;
	}
}

void CALL_C_a16()
{
	if(cpu.ca) {
		cpu.sp -= 2;
		write_word(cpu.sp, cpu.pc + 2);
		cpu.pc = read_word(cpu.pc);
		cpu.t += 24;
		cpu.last_t = 24;
	} else {
		cpu.pc += 2;
		cpu.t += 12;
		cpu.last_t = 12;
	}
}

void RST_18H()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc);
	cpu.pc = 0x0018;
	cpu.t += 16;
	cpu.last_t = 16;
}

void RST_20H()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc);
	cpu.pc = 0x0020;
	cpu.t += 16;
	cpu.last_t = 16;
}

void JP_HL()
{
	cpu.pc = read_word(cpu.hl);
	cpu.t += 4;
	cpu.last_t = 4;
}

void RST_28H()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc);
	cpu.pc = 0x0028;
	cpu.t += 16;
	cpu.last_t = 16;
}

void RST_30H()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc);
	cpu.pc = 0x0030;
	cpu.t += 16;
	cpu.last_t = 16;
}

void RST_38H()
{
	cpu.sp -= 2;
	write_word(cpu.sp, cpu.pc);
	cpu.pc = 0x0038;
	cpu.t += 16;
	cpu.last_t = 16;
}
