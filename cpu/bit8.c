/*
 * bit8.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../cpu.h"
#include "../mmu.h"
#include "bit8.h"
#include "opcode_table.h"

void RLCA()
{
	cpu.ca = (cpu.a >> 7) & 0x01;
	cpu.a <<= 1;
	cpu.a = (cpu.ca) ? cpu.a | 0x01 : cpu.a & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.a) ? 0 : 1;

	cpu.t += 4;
	cpu.last_t = 4;
}

void RRCA()
{
	cpu.ca = cpu.a & 0x01;
	cpu.a >>= 1;
	cpu.a = (cpu.ca) ? cpu.a | (0x01 << 7) : cpu.a & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.a) ? 0 : 1;

	cpu.t += 4;
	cpu.last_t = 4;
}

void RLA()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.a >> 7) & 0x01;
	cpu.a <<= 1;
	cpu.a = (temp) ? cpu.a | 0x01 : cpu.a & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.a) ? 0 : 1;

	cpu.t += 4;
	cpu.last_t = 4;
}

void RRA()
{
	byte temp = cpu.ca;
	cpu.ca = cpu.a & 0x01;
	cpu.a >>= 1;
	cpu.a = (temp) ? cpu.a | (0x01 << 7) : cpu.a & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 4;
	cpu.last_t = 4;
}

void PREFIX_CB()
{
	byte instruction = read_byte(cpu.pc);
	cpu.pc++;
	cb_prefix_table[instruction]();
	cpu.t += 4;
	cpu.last_t += 4;
}

void RLC_B()
{
	cpu.ca = (cpu.b >> 7) & 0x01;
	cpu.b <<= 1;
	cpu.b = (cpu.ca) ? cpu.b | 0x01 : cpu.b & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.b) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RLC_C()
{
	cpu.ca = (cpu.c >> 7) & 0x01;
	cpu.c <<= 1;
	cpu.c = (cpu.ca) ? cpu.c | 0x01 : cpu.c & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.c) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RLC_D()
{
	cpu.ca = (cpu.d >> 7) & 0x01;
	cpu.d <<= 1;
	cpu.d = (cpu.ca) ? cpu.d | 0x01 : cpu.d & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.d) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RLC_E()
{
	cpu.ca = (cpu.e >> 7) & 0x01;
	cpu.e <<= 1;
	cpu.e = (cpu.ca) ? cpu.e | 0x01 : cpu.e & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.e) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RLC_H()
{
	cpu.ca = (cpu.h >> 7) & 0x01;
	cpu.h <<= 1;
	cpu.h = (cpu.ca) ? cpu.h | 0x01 : cpu.h & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.h) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RLC_L()
{
	cpu.ca = (cpu.l >> 7) & 0x01;
	cpu.l <<= 1;
	cpu.l = (cpu.ca) ? cpu.l | 0x01 : cpu.l & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.l) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RLC_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.ca = (ihl >> 7) & 0x01;
	ihl <<= 1;
	ihl = (cpu.ca) ? ihl | 0x01 : ihl & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (ihl) ? 0 : 1;

	write_byte(cpu.hl, ihl);

	cpu.t += 16;
	cpu.last_t = 16;
}

void RLC_A()
{
	cpu.ca = (cpu.a >> 7) & 0x01;
	cpu.a <<= 1;
	cpu.a = (cpu.ca) ? cpu.a | 0x01 : cpu.a & (~0x01); // Set or reset the first bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.a) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RRC_B()
{
	cpu.ca = cpu.b & 0x01;
	cpu.b >>= 1;
	cpu.b = (cpu.ca) ? cpu.b | (0x01 << 7) : cpu.b & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.b) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RRC_C()
{
	cpu.ca = cpu.c & 0x01;
	cpu.c >>= 1;
	cpu.c = (cpu.ca) ? cpu.c | (0x01 << 7) : cpu.c & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.c) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RRC_D()
{
	cpu.ca = cpu.d & 0x01;
	cpu.d >>= 1;
	cpu.d = (cpu.ca) ? cpu.d | (0x01 << 7) : cpu.d & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.d) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RRC_E()
{
	cpu.ca = cpu.e & 0x01;
	cpu.e >>= 1;
	cpu.e = (cpu.ca) ? cpu.e | (0x01 << 7) : cpu.e & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.e) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RRC_H()
{
	cpu.ca = cpu.h & 0x01;
	cpu.h >>= 1;
	cpu.h = (cpu.ca) ? cpu.h | (0x01 << 7) : cpu.h & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.h) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RRC_L()
{
	cpu.ca = cpu.l & 0x01;
	cpu.l >>= 1;
	cpu.l = (cpu.ca) ? cpu.l | (0x01 << 7) : cpu.l & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.l) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RRC_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.ca = ihl & 0x01;
	ihl >>= 1;
	ihl = (cpu.ca) ? ihl | (0x01 << 7) : ihl & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (ihl) ? 0 : 1;

	write_byte(cpu.hl, ihl);

	cpu.t += 16;
	cpu.last_t = 16;
}

void RRC_A()
{
	cpu.ca = cpu.a & 0x01;
	cpu.a >>= 1;
	cpu.a = (cpu.ca) ? cpu.a | (0x01 << 7) : cpu.a & (~(0x01 << 7)); // Set or reset the 7th bit depending on what the was rotated out

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.a) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RL_B()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.b >> 7) & 0x01;
	cpu.b <<= 1;
	cpu.b = (temp) ? cpu.b | 0x01 : cpu.b & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.b) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RL_C()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.c >> 7) & 0x01;
	cpu.c <<= 1;
	cpu.c = (temp) ? cpu.c | 0x01 : cpu.c & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.c) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RL_D()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.d >> 7) & 0x01;
	cpu.d <<= 1;
	cpu.d = (temp) ? cpu.d | 0x01 : cpu.d & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.d) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RL_E()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.e >> 7) & 0x01;
	cpu.e <<= 1;
	cpu.e = (temp) ? cpu.e | 0x01 : cpu.e & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.e) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RL_H()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.h >> 7) & 0x01;
	cpu.h <<= 1;
	cpu.h = (temp) ? cpu.h | 0x01 : cpu.h & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.h) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RL_L()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.l >> 7) & 0x01;
	cpu.l <<= 1;
	cpu.l = (temp) ? cpu.l | 0x01 : cpu.l & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.l) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RL_IHL()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.d >> 7) & 0x01;
	cpu.d <<= 1;
	cpu.d = (temp) ? cpu.d | 0x01 : cpu.d & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.d) ? 0 : 1;

	cpu.t += 16;
	cpu.last_t = 16;
}

void RL_A()
{
	byte temp = cpu.ca;
	cpu.ca = (cpu.a >> 7) & 0x01;
	cpu.a <<= 1;
	cpu.a = (temp) ? cpu.a | 0x01 : cpu.a & (~0x01);

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.a) ? 0 : 1;

	cpu.t += 8;
	cpu.last_t = 8;
}

void RR_B()
{
	byte temp = cpu.ca;
	cpu.ca = cpu.b & 0x01;
	cpu.b >>= 1;
	cpu.b = (temp) ? cpu.b | (0x01 << 7) : cpu.b & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.b) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RR_C()
{
	byte temp = cpu.ca;
	cpu.ca = cpu.c & 0x01;
	cpu.c >>= 1;
	cpu.c = (temp) ? cpu.c | (0x01 << 7) : cpu.c & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.c) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RR_D()
{
	byte temp = cpu.ca;
	cpu.ca = cpu.d & 0x01;
	cpu.d >>= 1;
	cpu.d = (temp) ? cpu.d | (0x01 << 7) : cpu.d & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.d) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RR_E()
{
	byte temp = cpu.ca;
	cpu.ca = cpu.e & 0x01;
	cpu.e >>= 1;
	cpu.e = (temp) ? cpu.e | (0x01 << 7) : cpu.e & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.e) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RR_H()
{
	byte temp = cpu.ca;
	cpu.ca = cpu.h & 0x01;
	cpu.h >>= 1;
	cpu.h = (temp) ? cpu.h | (0x01 << 7) : cpu.h & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.h) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RR_L()
{
	byte temp = cpu.ca;
	cpu.ca = cpu.l & 0x01;
	cpu.l >>= 1;
	cpu.l = (temp) ? cpu.l | (0x01 << 7) : cpu.l & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.l) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RR_IHL()
{
	byte ihl = read_byte(cpu.hl);
	byte temp = cpu.ca;
	cpu.ca = ihl & 0x01;
	ihl >>= 1;
	ihl = (temp) ? ihl | (0x01 << 7) : ihl & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (ihl) ? 0 : 1;
	write_byte(cpu.hl, ihl);

	cpu.t += 16;
	cpu.last_t = 16;
}

void RR_A()
{
	byte temp = cpu.ca;
	cpu.ca = cpu.a & 0x01;
	cpu.a >>= 1;
	cpu.a = (temp) ? cpu.a | (0x01 << 7) : cpu.a & (~(0x01 << 7));

	cpu.n = 0;
	cpu.hc = 0;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SLA_B()
{
	cpu.ca = (cpu.b >> 7) & 0x01;
	cpu.b <<= 1;
	cpu.z = (cpu.b) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SLA_C()
{
	cpu.ca = (cpu.c >> 7) & 0x01;
	cpu.c <<= 1;
	cpu.z = (cpu.c) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SLA_D()
{
	cpu.ca = (cpu.d >> 7) & 0x01;
	cpu.d <<= 1;
	cpu.z = (cpu.d) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SLA_E()
{
	cpu.ca = (cpu.e >> 7) & 0x01;
	cpu.e <<= 1;
	cpu.z = (cpu.e) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SLA_H()
{
	cpu.ca = (cpu.h >> 7) & 0x01;
	cpu.h <<= 1;
	cpu.z = (cpu.h) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SLA_L()
{
	cpu.ca = (cpu.l >> 7) & 0x01;
	cpu.l <<= 1;
	cpu.z = (cpu.l) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SLA_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.ca = (ihl >> 7) & 0x01;
	ihl <<= 1;
	cpu.z = (ihl) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SLA_A()
{
	cpu.ca = (cpu.a >> 7) & 0x01;
	cpu.a <<= 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRA_B()
{
	cpu.ca = cpu.b & 0x01;
	cpu.b >>= 1;
	cpu.z = (cpu.b) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRA_C()
{
	cpu.ca = cpu.c & 0x01;
	cpu.c >>= 1;
	cpu.z = (cpu.c) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRA_D()
{
	cpu.ca = cpu.d & 0x01;
	cpu.d >>= 1;
	cpu.z = (cpu.d) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRA_E()
{
	cpu.ca = cpu.e & 0x01;
	cpu.e >>= 1;
	cpu.z = (cpu.e) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRA_H()
{
	cpu.ca = cpu.h & 0x01;
	cpu.h >>= 1;
	cpu.z = (cpu.h) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRA_L()
{
	cpu.ca = cpu.l & 0x01;
	cpu.l >>= 1;
	cpu.z = (cpu.l) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRA_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.ca = ihl & 0x01;
	ihl >>= 1;
	cpu.z = (ihl) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SRA_A()
{
	cpu.ca = cpu.a & 0x01;
	cpu.a >>= 1;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SWAP_B()
{
	byte nibble = cpu.b & 0x0F;
	cpu.b >>= 4;
	cpu.b &= 0x0F;
	cpu.b |= nibble << 4;
	cpu.z = (cpu.b) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SWAP_C()
{
	byte nibble = cpu.c & 0x0F;
	cpu.c >>= 4;
	cpu.c &= 0x0F;
	cpu.c |= nibble << 4;
	cpu.z = (cpu.c) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SWAP_D()
{
	byte nibble = cpu.d & 0x0F;
	cpu.d >>= 4;
	cpu.d &= 0x0F;
	cpu.d |= nibble << 4;
	cpu.z = (cpu.d) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SWAP_E()
{
	byte nibble = cpu.e & 0x0F;
	cpu.e >>= 4;
	cpu.e &= 0x0F;
	cpu.e |= nibble << 4;
	cpu.z = (cpu.e) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SWAP_H()
{
	byte nibble = cpu.h & 0x0F;
	cpu.h >>= 4;
	cpu.h &= 0x0F;
	cpu.h |= nibble << 4;
	cpu.z = (cpu.h) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SWAP_L()
{
	byte nibble = cpu.l & 0x0F;
	cpu.l >>= 4;
	cpu.l &= 0x0F;
	cpu.l |= nibble << 4;
	cpu.z = (cpu.l) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SWAP_IHL()
{
	byte ihl = read_byte(cpu.hl);
	byte nibble = ihl & 0x0F;
	ihl >>= 4;
	ihl &= 0x0F;
	ihl |= nibble << 4;
	cpu.z = (ihl) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = 0;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SWAP_A()
{
	byte nibble = cpu.a & 0x0F;
	cpu.a >>= 4;
	cpu.a &= 0x0F;
	cpu.a |= nibble << 4;
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.ca = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRL_B()
{
	cpu.ca = cpu.b & 0x01;
	cpu.b >>= 1;
	cpu.b &= ~(0x01 << 7);
	cpu.z = (cpu.b) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRL_C()
{
	cpu.ca = cpu.c & 0x01;
	cpu.c >>= 1;
	cpu.c &= ~(0x01 << 7);
	cpu.z = (cpu.c) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRL_D()
{
	cpu.ca = cpu.d & 0x01;
	cpu.d >>= 1;
	cpu.d &= ~(0x01 << 7);
	cpu.z = (cpu.d) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRL_E()
{
	cpu.ca = cpu.e & 0x01;
	cpu.e >>= 1;
	cpu.e &= ~(0x01 << 7);
	cpu.z = (cpu.e) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRL_H()
{
	cpu.ca = cpu.h & 0x01;
	cpu.h >>= 1;
	cpu.h &= ~(0x01 << 7);
	cpu.z = (cpu.h) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRL_L()
{
	cpu.ca = cpu.l & 0x01;
	cpu.l >>= 1;
	cpu.l &= ~(0x01 << 7);
	cpu.z = (cpu.l) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SRL_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.ca = ihl & 0x01;
	ihl >>= 1;
	ihl &= ~(0x01 << 7);
	cpu.z = (ihl) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SRL_A()
{
	cpu.ca = cpu.a & 0x01;
	cpu.a >>= 1;
	cpu.a &= ~(0x01 << 7);
	cpu.z = (cpu.a) ? 0 : 1;
	cpu.n = 0;
	cpu.hc = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_0_B()
{
	cpu.z = !cpu.b0;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_0_C()
{
	cpu.z = !cpu.c0;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_0_D()
{
	cpu.z = !cpu.d0;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_0_E()
{
	cpu.z = !cpu.e0;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_0_H()
{
	cpu.z = !cpu.h0;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_0_L()
{
	cpu.z = !cpu.l0;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_0_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.z = !((ihl >> 0) & 0x01);
	cpu.n = 0;
	cpu.hc = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void BIT_0_A()
{
	cpu.z = !cpu.a0;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_1_B()
{
	cpu.z = !cpu.b1;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_1_C()
{
	cpu.z = !cpu.c1;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_1_D()
{
	cpu.z = !cpu.d1;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_1_E()
{
	cpu.z = !cpu.e1;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_1_H()
{
	cpu.z = !cpu.h1;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_1_L()
{
	cpu.z = !cpu.l1;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_1_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.z = !((ihl >> 1) & 0x01);
	cpu.n = 0;
	cpu.hc = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void BIT_1_A()
{
	cpu.z = !cpu.a1;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_2_B()
{
	cpu.z = !cpu.b2;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_2_C()
{
	cpu.z = !cpu.c2;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_2_D()
{
	cpu.z = !cpu.d2;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_2_E()
{
	cpu.z = !cpu.e2;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_2_H()
{
	cpu.z = !cpu.h2;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_2_L()
{
	cpu.z = !cpu.l2;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_2_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.z = !((ihl >> 2) & 0x01);
	cpu.n = 0;
	cpu.hc = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void BIT_2_A()
{
	cpu.z = !cpu.a2;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_3_B()
{
	cpu.z = !cpu.b3;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_3_C()
{
	cpu.z = !cpu.c3;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_3_D()
{
	cpu.z = !cpu.d3;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_3_E()
{
	cpu.z = !cpu.e3;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_3_H()
{
	cpu.z = !cpu.h3;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_3_L()
{
	cpu.z = !cpu.l3;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_3_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.z = !((ihl >> 3) & 0x01);
	cpu.n = 0;
	cpu.hc = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void BIT_3_A()
{
	cpu.z = !cpu.a3;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_4_B()
{
	cpu.z = !cpu.b4;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_4_C()
{
	cpu.z = !cpu.c4;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_4_D()
{
	cpu.z = !cpu.d4;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_4_E()
{
	cpu.z = !cpu.e4;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_4_H()
{
	cpu.z = !cpu.h4;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_4_L()
{
	cpu.z = !cpu.l4;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_4_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.z = !((ihl >> 4) & 0x01);
	cpu.n = 0;
	cpu.hc = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void BIT_4_A()
{
	cpu.z = !cpu.a4;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_5_B()
{
	cpu.z = !cpu.b5;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_5_C()
{
	cpu.z = !cpu.c5;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_5_D()
{
	cpu.z = !cpu.d5;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_5_E()
{
	cpu.z = !cpu.e5;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_5_H()
{
	cpu.z = !cpu.h5;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_5_L()
{
	cpu.z = !cpu.l5;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_5_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.z = !((ihl >> 5) & 0x01);
	cpu.n = 0;
	cpu.hc = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void BIT_5_A()
{
	cpu.z = !cpu.a5;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_6_B()
{
	cpu.z = !cpu.b6;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_6_C()
{
	cpu.z = !cpu.c6;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_6_D()
{
	cpu.z = !cpu.d6;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_6_E()
{
	cpu.z = !cpu.e6;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_6_H()
{
	cpu.z = !cpu.h6;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_6_L()
{
	cpu.z = !cpu.l6;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_6_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.z = !((ihl >> 6) & 0x01);
	cpu.n = 0;
	cpu.hc = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void BIT_6_A()
{
	cpu.z = !cpu.a6;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_7_B()
{
	cpu.z = !cpu.b7;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_7_C()
{
	cpu.z = !cpu.c7;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_7_D()
{
	cpu.z = !cpu.d7;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_7_E()
{
	cpu.z = !cpu.e7;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_7_H()
{
	cpu.z = !cpu.h7;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_7_L()
{
	cpu.z = !cpu.l7;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void BIT_7_IHL()
{
	byte ihl = read_byte(cpu.hl);
	cpu.z = !((ihl >> 7) & 0x01);
	cpu.n = 0;
	cpu.hc = 1;
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void BIT_7_A()
{
	cpu.z = !cpu.a7;
	cpu.n = 0;
	cpu.hc = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_0_B()
{
	cpu.b0 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_0_C()
{
	cpu.c0 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_0_D()
{
	cpu.d0 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_0_E()
{
	cpu.e0 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_0_H()
{
	cpu.h0 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_0_L()
{
	cpu.l0 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_0_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl &= ~(0x01 << 0);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void RES_0_A()
{
	cpu.a0 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_1_B()
{
	cpu.b1 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_1_C()
{
	cpu.c1 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_1_D()
{
	cpu.d1 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_1_E()
{
	cpu.e1 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_1_H()
{
	cpu.h1 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_1_L()
{
	cpu.l1 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_1_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl &= ~(0x01 << 1);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void RES_1_A()
{
	cpu.a1 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_2_B()
{
	cpu.b2 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_2_C()
{
	cpu.c2 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_2_D()
{
	cpu.d2 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_2_E()
{
	cpu.e2 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_2_H()
{
	cpu.h2 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_2_L()
{
	cpu.l2 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_2_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl &= ~(0x01 << 2);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void RES_2_A()
{
	cpu.a2 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_3_B()
{
	cpu.b3 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_3_C()
{
	cpu.c3 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_3_D()
{
	cpu.d3 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_3_E()
{
	cpu.e3 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_3_H()
{
	cpu.h3 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_3_L()
{
	cpu.l3 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_3_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl &= ~(0x01 << 3);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void RES_3_A()
{
	cpu.a3 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_4_B()
{
	cpu.b4 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_4_C()
{
	cpu.c4 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_4_D()
{
	cpu.d4 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_4_E()
{
	cpu.e4 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_4_H()
{
	cpu.h4 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_4_L()
{
	cpu.l4 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_4_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl &= ~(0x01 << 4);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void RES_4_A()
{
	cpu.a4 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_5_B()
{
	cpu.b5 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_5_C()
{
	cpu.c5 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_5_D()
{
	cpu.d5 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_5_E()
{
	cpu.e5 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_5_H()
{
	cpu.h5 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_5_L()
{
	cpu.l5 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_5_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl &= ~(0x01 << 5);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void RES_5_A()
{
	cpu.a5 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_6_B()
{
	cpu.b6 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_6_C()
{
	cpu.c6 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_6_D()
{
	cpu.d6 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_6_E()
{
	cpu.e6 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_6_H()
{
	cpu.h6 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_6_L()
{
	cpu.l6 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_6_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl &= ~(0x01 << 6);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void RES_6_A()
{
	cpu.a6 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_7_B()
{
	cpu.b7 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_7_C()
{
	cpu.c7 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_7_D()
{
	cpu.d7 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_7_E()
{
	cpu.e7 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_7_H()
{
	cpu.h7 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_7_L()
{
	cpu.l7 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void RES_7_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl &= ~(0x01 << 7);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void RES_7_A()
{
	cpu.a7 = 0;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_0_B()
{
	cpu.b0 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_0_C()
{
	cpu.c0 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_0_D()
{
	cpu.d0 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_0_E()
{
	cpu.e0 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_0_H()
{
	cpu.h0 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_0_L()
{
	cpu.l0 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_0_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl |= (0x01 << 0);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SET_0_A()
{
	cpu.a0 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_1_B()
{
	cpu.b1 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_1_C()
{
	cpu.c1 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_1_D()
{
	cpu.d1 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_1_E()
{
	cpu.e1 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_1_H()
{
	cpu.h1 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_1_L()
{
	cpu.l1 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_1_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl |= (0x01 << 1);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SET_1_A()
{
	cpu.a1 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_2_B()
{
	cpu.b2 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_2_C()
{
	cpu.c2 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_2_D()
{
	cpu.d2 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_2_E()
{
	cpu.e2 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_2_H()
{
	cpu.h2 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_2_L()
{
	cpu.l2 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_2_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl |= (0x01 << 2);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SET_2_A()
{
	cpu.a2 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_3_B()
{
	cpu.b3 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_3_C()
{
	cpu.c3 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_3_D()
{
	cpu.d3 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_3_E()
{
	cpu.e3 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_3_H()
{
	cpu.h3 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_3_L()
{
	cpu.l3 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_3_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl |= (0x01 << 3);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SET_3_A()
{
	cpu.a3 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_4_B()
{
	cpu.b4 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_4_C()
{
	cpu.c4 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_4_D()
{
	cpu.d4 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_4_E()
{
	cpu.e4 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_4_H()
{
	cpu.h4 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_4_L()
{
	cpu.l4 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_4_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl |= (0x01 << 4);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SET_4_A()
{
	cpu.a4 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_5_B()
{
	cpu.b5 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_5_C()
{
	cpu.c5 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_5_D()
{
	cpu.d5 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_5_E()
{
	cpu.e5 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_5_H()
{
	cpu.h5 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_5_L()
{
	cpu.l5 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_5_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl |= (0x01 << 5);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SET_5_A()
{
	cpu.a5 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_6_B()
{
	cpu.b6 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_6_C()
{
	cpu.c6 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_6_D()
{
	cpu.d6 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_6_E()
{
	cpu.e6 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_6_H()
{
	cpu.h6 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_6_L()
{
	cpu.l6 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_6_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl |= (0x01 << 6);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SET_6_A()
{
	cpu.a6 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_7_B()
{
	cpu.b7 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_7_C()
{
	cpu.c7 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_7_D()
{
	cpu.d7 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_7_E()
{
	cpu.e7 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_7_H()
{
	cpu.h7 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_7_L()
{
	cpu.l7 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}

void SET_7_IHL()
{
	byte ihl = read_byte(cpu.hl);
	ihl |= (0x01 << 7);
	write_byte(cpu.hl, ihl);
	cpu.t += 16;
	cpu.last_t = 16;
}

void SET_7_A()
{
	cpu.a7 = 1;
	cpu.t += 8;
	cpu.last_t = 8;
}
