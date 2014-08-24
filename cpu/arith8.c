/*
 * arith8.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../cpu.h"
#include "../mmu.h"
#include "arith8.h"

void INC_B()
{
	cpu.hc = (~(cpu.b & 0x0F)) ? 0 : 1; // Carry only if lower nibble is all 1's
	cpu.b++;
	cpu.z = (cpu.b) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DEC_B()
{
	cpu.hc = (cpu.b & 0x0F) ? 1 : 0; // Borrow only occurs if the lower nibble is 0
	cpu.b--;
	cpu.z = (cpu.b) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void INC_C()
{
	cpu.hc = (~(cpu.c & 0x0F)) ? 0 : 1; // Carry only if lower nibble is all 1's
	cpu.c++;
	cpu.z = (cpu.c) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DEC_C()
{
	cpu.hc = (cpu.c & 0x0F) ? 1 : 0; // Borrow only occurs if the lower nibble is 0
	cpu.c--;
	cpu.z = (cpu.c) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void INC_D()
{
	cpu.hc = (~(cpu.d & 0x0F)) ? 0 : 1; // Carry only if lower nibble is all 1's
	cpu.d++;
	cpu.z = (cpu.d) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DEC_D()
{
	cpu.hc = (cpu.d & 0x0F) ? 1 : 0; // Borrow only occurs if the lower nibble is 0
	cpu.d--;
	cpu.z = (cpu.d) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void INC_E()
{
	cpu.hc = (~(cpu.e & 0x0F)) ? 0 : 1; // Carry only if lower nibble is all 1's
	cpu.e++;
	cpu.z = (cpu.e) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DEC_E()
{
	cpu.hc = (cpu.e & 0x0F) ? 1 : 0; // Borrow only occurs if the lower nibble is 0
	cpu.e--;
	cpu.z = (cpu.e) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void INC_H()
{
	cpu.hc = (~(cpu.h & 0x0F)) ? 0 : 1; // Carry only if lower nibble is all 1's
	cpu.h++;
	cpu.z = (cpu.h) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DEC_H()
{
	cpu.hc = (cpu.h & 0x0F) ? 1 : 0; // Borrow only occurs if the lower nibble is 0
	cpu.h--;
	cpu.z = (cpu.h) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DAA()
{
	// TODO: implement DAA
}

void INC_L()
{
	cpu.hc = (~(cpu.l & 0x0F)) ? 0 : 1; // Carry only if lower nibble is all 1's
	cpu.l++;
	cpu.z = (cpu.l) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DEC_L()
{
	cpu.hc = (cpu.l & 0x0F) ? 1 : 0; // Borrow only occurs if the lower nibble is 0
	cpu.l--;
	cpu.z = (cpu.l) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CPL()
{
	cpu.a = ~cpu.a;
	cpu.n = 1;
	cpu.hc = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void INC_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.hc = (~(ihl & 0x0F)) ? 0 : 1; // Carry only if lower nibble is all 1's
	ihl++;
	cpu.z = (ihl) ? 0 : 1;
	cpu.n = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 12;
	cpu.last_t = 12;
}

void DEC_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.hc = (ihl & 0x0F) ? 1 : 0; // Borrow only occurs if the lower nibble is 0
	ihl--;
	cpu.z = (ihl) ? 0 : 1;
	cpu.n = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 12;
	cpu.last_t = 12;
}

void SCF()
{
	cpu.ca = 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void INC_A()
{
	cpu.hc = (~(cpu.a & 0x0F)) ? 0 : 1; // Carry only if lower nibble is all 1's
	cpu.a++;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DEC_A()
{
	cpu.hc = (cpu.a & 0x0F) ? 1 : 0; // Borrow only occurs if the lower nibble is 0
	cpu.a--;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CCF()
{
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = ~cpu.ca;
}

void ADD_A_B()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.b;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADD_A_C()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.c;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADD_A_D()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.d;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADD_A_E()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.e;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADD_A_H()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.h;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADD_A_L()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.l;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADD_A_IHL()
{
	byte ihl = read_byte(cpu.hl);
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += ihl;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void ADD_A_A()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.a;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADC_A_B()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.b + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADC_A_C()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.c + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADC_A_D()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.d + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADC_A_E()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.e + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADC_A_H()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.h + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADC_A_L()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.l + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADC_A_IHL()
{
	byte ihl = read_byte(cpu.hl);
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += ihl + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void ADC_A_A()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += cpu.a + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SUB_B()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.b;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SUB_C()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.c;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SUB_D()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.d;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SUB_E()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.e;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SUB_H()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.h;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SUB_L()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.h;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SUB_IHL()
{
	byte ihl = read_byte(cpu.hl);
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= ihl;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SUB_A()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.a;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SBC_A_B()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.b + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SBC_A_C()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.c + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SBC_A_D()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.d + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SBC_A_E()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.e + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SBC_A_H()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.h + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SBC_A_L()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.l + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void SBC_A_IHL()
{
	byte ihl = read_byte(cpu.hl);
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= ihl + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SBC_A_A()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= cpu.a + cpu.ca;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void AND_B()
{
	cpu.a &= cpu.b;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void AND_C()
{
	cpu.a &= cpu.c;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void AND_D()
{
	cpu.a &= cpu.d;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void AND_E()
{
	cpu.a &= cpu.e;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void AND_H()
{
	cpu.a &= cpu.h;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void AND_L()
{
	cpu.a &= cpu.l;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void AND_IHL()
{
	cpu.a &= read_byte(cpu.hl);
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void AND_A()
{
	cpu.a &= cpu.a;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XOR_B()
{
	cpu.a ^= cpu.b;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XOR_C()
{
	cpu.a ^= cpu.c;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XOR_D()
{
	cpu.a ^= cpu.d;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XOR_E()
{
	cpu.a ^= cpu.e;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XOR_H()
{
	cpu.a ^= cpu.h;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XOR_L()
{
	cpu.a ^= cpu.l;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XOR_IHL()
{
	cpu.a ^= read_byte(cpu.hl);
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void XOR_A()
{ // Optimize this - always spits out 0
	cpu.a ^= cpu.a;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void OR_B()
{
	cpu.a |= cpu.b;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void OR_C()
{
	cpu.a |= cpu.c;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void OR_D()
{
	cpu.a |= cpu.d;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void OR_E()
{
	cpu.a |= cpu.e;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void OR_H()
{
	cpu.a |= cpu.h;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void OR_L()
{
	cpu.a |= cpu.l;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void OR_IHL()
{
	cpu.a |= read_byte(cpu.hl);
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void OR_A()
{
	cpu.a |= cpu.a;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CP_B()
{
	byte after = cpu.a - cpu.b;
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CP_C()
{
	byte after = cpu.a - cpu.c;
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CP_D()
{
	byte after = cpu.a - cpu.d;
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CP_E()
{
	byte after = cpu.a - cpu.e;
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CP_H()
{
	byte after = cpu.a - cpu.h;
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CP_L()
{
	byte after = cpu.a - cpu.l;
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void CP_IHL()
{
	byte after = cpu.a - read_byte(cpu.hl);
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void CP_A()
{
	byte after = cpu.a - cpu.a;
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void ADD_A_d8()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += read_byte(cpu.pc);
	cpu.pc++;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void ADC_A_d8()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a += read_byte(cpu.pc) + cpu.ca;
	cpu.pc++;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0;
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu.n = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SUB_d8()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= read_byte(cpu.pc);
	cpu.pc++;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SBC_A_d8()
{
	byte bef_byte = cpu.a;
	byte bef = cpu.a & 0x0F;
	cpu.a -= read_byte(cpu.pc) + cpu.ca;
	cpu.pc++;
	cpu.hc = ((cpu.a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void AND_d8()
{
	cpu.a &= read_byte(cpu.pc);
	cpu.pc++;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 1;
	cpu.ca = 0;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XOR_d8()
{
	cpu.a ^= read_byte(cpu.pc);
	cpu.pc++;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void OR_d8()
{
	cpu.a |= read_byte(cpu.pc);
	cpu.pc++;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.h = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void CP_d8()
{
	byte after = cpu.a - read_byte(cpu.pc);
	cpu.pc++;
	cpu.hc = ((cpu.a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu.ca = (cpu.a < after) ? 1 : 0; // Set if no borrow
	cpu.n = 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}
