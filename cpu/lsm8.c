/*
 * lsm8.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../cpu.h"
#include "../mmu.h"
#include "lsm8.h"

void LD_BC_A()
{
	write_byte(cpu.bc, cpu.a);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_B_d8()
{
	cpu.b = read_byte(cpu.pc);
	cpu.pc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_A_BC()
{
	cpu.a = read_byte(cpu.bc);
	cpu.pc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_C_d8()
{
	cpu.c = read_byte(cpu.pc);
	cpu.pc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_DE_A()
{
	write_byte(cpu.de, cpu.a);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_D_d8()
{
	cpu.d = read_byte(cpu.pc);
	cpu.pc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_A_DE()
{
	cpu.a = read_byte(cpu.de);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_E_d8()
{
	cpu.e = read_byte(cpu.pc);
	cpu.pc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LDI_HL_A()
{
	write_byte(cpu.hl, cpu.a);
	cpu.hl++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_H_d8()
{
	cpu.h = read_byte(cpu.pc);
	cpu.pc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LDI_A_HL()
{
	cpu.a = read_byte(cpu.hl);
	cpu.hl++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_L_d8()
{
	cpu.l = read_byte(cpu.pc);
	cpu.pc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LDD_HL_A()
{
	write_byte(cpu.hl, cpu.a);
	cpu.hl--;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_IHL_d8()
{
	write_byte(cpu.hl, read_byte(cpu.pc));
	cpu.pc++;
	cpu.t += 12;
	cpu.last_t = 12;
}

void LDD_A_HL()
{
	cpu.a = read_byte(cpu.hl);
	cpu.hl--;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_A_d8()
{
	cpu.a = read_byte(cpu.pc);
	cpu.pc++;
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_B_B()
{

	// Do nothing

	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_B_C()
{
	cpu.b = cpu.c;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_B_D()
{
	cpu.b = cpu.d;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_B_E()
{
	cpu.b = cpu.e;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_B_H()
{
	cpu.b = cpu.h;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_B_L()
{
	cpu.b = cpu.l;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_B_IHL()
{
	cpu.b = read_byte(cpu.hl);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_B_A()
{
	cpu.b = cpu.a;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_C_B()
{
	cpu.c = cpu.b;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_C_C()
{

	// Do nothing

	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_C_D()
{
	cpu.c = cpu.d;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_C_E()
{
	cpu.c = cpu.e;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_C_H()
{
	cpu.c = cpu.h;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_C_L()
{
	cpu.c = cpu.l;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_C_IHL()
{
	cpu.c = read_byte(cpu.hl);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_C_A()
{
	cpu.c = cpu.a;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_D_B()
{
	cpu.d = cpu.b;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_D_C()
{
	cpu.d = cpu.c;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_D_D()
{

	// Do nothing

	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_D_E()
{
	cpu.d = cpu.e;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_D_H()
{
	cpu.d = cpu.h;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_D_L()
{
	cpu.d = cpu.l;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_D_IHL()
{
	cpu.d = read_byte(cpu.hl);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_D_A()
{
	cpu.d = cpu.a;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_E_B()
{
	cpu.e = cpu.b;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_E_C()
{
	cpu.e = cpu.c;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_E_D()
{
	cpu.e = cpu.d;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_E_E()
{
	// Do nothing

	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_E_H()
{
	cpu.e = cpu.h;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_E_L()
{
	cpu.e = cpu.l;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_E_IHL()
{
	cpu.e = read_byte(cpu.hl);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_E_A()
{
	cpu.e = cpu.a;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_H_B()
{
	cpu.h = cpu.b;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_H_C()
{
	cpu.h = cpu.c;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_H_D()
{
	cpu.h = cpu.d;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_H_E()
{
	cpu.h = cpu.e;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_H_H()
{
	// Do nothing

	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_H_L()
{
	cpu.h = cpu.l;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_H_IHL()
{
	cpu.h = read_byte(cpu.hl);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_H_A()
{
	cpu.h = cpu.a;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_L_B()
{
	cpu.l = cpu.b;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_L_C()
{
	cpu.l = cpu.c;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_L_D()
{
	cpu.l = cpu.d;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_L_E()
{
	cpu.l = cpu.e;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_L_H()
{
	cpu.l = cpu.h;
	cpu.t += 4;
	cpu.last_t = 4;

}

void LD_L_L()
{
	// Do nothing

	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_L_IHL()
{
	cpu.l = read_byte(cpu.hl);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_L_A()
{
	cpu.l = cpu.a;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_IHL_B()
{
	write_byte(cpu.hl, cpu.b);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_IHL_C()
{
	write_byte(cpu.hl, cpu.c);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_IHL_D()
{
	write_byte(cpu.hl, cpu.d);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_IHL_E()
{
	write_byte(cpu.hl, cpu.e);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_IHL_H()
{
	write_byte(cpu.hl, cpu.h);
	cpu.t += 8;
	cpu.last_t = 8;

}

void LD_IHL_L()
{
	write_byte(cpu.hl, cpu.l);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_IHL_A()
{
	write_byte(cpu.hl, cpu.a);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_A_B()
{
	cpu.a = cpu.b;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_A_C()
{
	cpu.a = cpu.c;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_A_D()
{
	cpu.a = cpu.d;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_A_E()
{
	cpu.a = cpu.e;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_A_H()
{
	cpu.a = cpu.h;
	cpu.t += 4;
	cpu.last_t = 4;

}

void LD_A_L()
{
	cpu.a = cpu.l;
	cpu.t += 4;
	cpu.last_t = 4;
}

void LD_A_IHL()
{
	cpu.a = read_byte(cpu.hl);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_A_A()
{
	// Do nothing

	cpu.t += 4;
	cpu.last_t = 4;
}

void LDH_a8_A()
{
	write_byte(0xFF00 + read_byte(cpu.pc), cpu.a);
	cpu.pc++;
	cpu.t += 12;
	cpu.last_t = 12;
}

void LD_IC_A()
{
	write_byte(0xFF00 + cpu.c, cpu.a);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_a16_A()
{
	write_byte(read_word(cpu.pc), cpu.a);
	cpu.pc += 2;
	cpu.t += 16;
	cpu.last_t = 16;
}

void LDH_A_a8()
{
	cpu.a = read_byte(0xFF00 + read_byte(cpu.pc));
	cpu.pc++;
	cpu.t += 12;
	cpu.last_t = 12;
}

void LD_A_IC()
{
	cpu.a = read_byte(0xFF00 + cpu.c);
	cpu.t += 8;
	cpu.last_t = 8;
}

void LD_A_a16()
{
	cpu.a = read_byte(read_word(cpu.pc));
	cpu.pc += 2;
	cpu.t += 16;
	cpu.last_t = 16;
}

