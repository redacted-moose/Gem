/*
 * arith16.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../cpu.h"
#include "../mmu.h"
#include "arith16.h"

void INC_BC()
{
	cpu.bc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void ADD_HL_BC()
{
	word before = cpu.hl;
	cpu.hl += cpu.bc;
	cpu.n = 0;
	cpu.hc = ((before & 0x0FFF) > (cpu.hl & 0x0FFF))? 1 : 0;
	cpu.ca = (before > cpu.hl)? 1 : 0;
}

void DEC_BC()
{
	cpu.bc--;
	cpu.t += 8;
	cpu.last_t = 8;
}

void INC_DE()
{
	cpu.de++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void ADD_HL_DE()
{
	word before = cpu.hl;
	cpu.hl += cpu.de;
	cpu.n = 0;
	cpu.hc = ((before & 0x0FFF) > (cpu.hl & 0x0FFF))? 1 : 0;
	cpu.ca = (before > cpu.hl)? 1 : 0;
}

void DEC_DE()
{
	cpu.de--;
	cpu.t += 8;
	cpu.last_t = 8;
}

void INC_HL()
{
	cpu.hl++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void ADD_HL_HL()
{
	word before = cpu.hl;
	cpu.hl += cpu.hl;
	cpu.n = 0;
	cpu.hc = ((before & 0x0FFF) > (cpu.hl & 0x0FFF))? 1 : 0;
	cpu.ca = (before > cpu.hl)? 1 : 0;
}

void DEC_HL()
{
	cpu.hl--;
	cpu.t += 8;
	cpu.last_t = 8;
}

void INC_SP()
{
	cpu.sp++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void ADD_HL_SP()
{
	word before = cpu.hl;
	cpu.hl += cpu.sp;
	cpu.n = 0;
	cpu.hc = ((before & 0x0FFF) > (cpu.hl & 0x0FFF))? 1 : 0;
	cpu.ca = (before > cpu.hl)? 1 : 0;
}

void DEC_SP()
{
	cpu.sp--;
	cpu.t += 8;
	cpu.last_t = 8;
}

void ADD_SP_r8()
{
	word before = cpu.sp;
	cpu.sp += read_byte(cpu.pc);
	cpu.pc++;
	cpu.z = 0;
	cpu.n = 0;
	cpu.hc = ((before & 0x0FFF) > (cpu.hl & 0x0FFF))? 1 : 0;
	cpu.ca = (before > cpu.hl)? 1 : 0;
}
