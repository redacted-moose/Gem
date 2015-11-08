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

void RLCA(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->ca = (cpu->a >> 7) & 0x01;
	cpu->a <<= 1;
    // Set or reset the first bit depending on what the was rotated out
	cpu->a = (cpu->ca) ? cpu->a | 0x01 : cpu->a & (~0x01);

	cpu->n = 0;
	cpu->hc = 0;
	cpu->z = (cpu->a) ? 0 : 1;
}

void RRCA(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->ca = cpu->a & 0x01;
	cpu->a >>= 1;
    // Set or reset the 7th bit depending on what the was rotated out
	cpu->a = (cpu->ca) ? cpu->a | (0x01 << 7) : cpu->a & (~(0x01 << 7));

	cpu->n = 0;
	cpu->hc = 0;
	cpu->z = (cpu->a) ? 0 : 1;
}

void RLA(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	byte temp = cpu->ca;
	cpu->ca = (cpu->a >> 7) & 0x01;
	cpu->a <<= 1;
	cpu->a = (temp) ? cpu->a | 0x01 : cpu->a & (~0x01);

	cpu->n = 0;
	cpu->hc = 0;
	cpu->z = (cpu->a) ? 0 : 1;
}

void RRA(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	byte temp = cpu->ca;
	cpu->ca = cpu->a & 0x01;
	cpu->a >>= 1;
	cpu->a = (temp) ? cpu->a | (0x01 << 7) : cpu->a & (~(0x01 << 7));

	cpu->n = 0;
	cpu->hc = 0;
	cpu->z = (cpu->a) ? 0 : 1;
}

void PREFIX_CB(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	byte instruction = read_byte(gem, cpu->pc);
    INFO("CB instruction: 0x%02X\n", instruction);
	cpu->pc++;
	cb_prefix_table[instruction].execute(gem);
	cpu->t += cb_prefix_table[instruction].timing1;
	cpu->last_t += cb_prefix_table[instruction].timing1;
}


#define RLC_R8(REG8, reg8)                                       \
    void RLC_ ## REG8 (struct machine_t *gem) {                         \
        struct cpu_t *cpu = gem->cpu;                                   \
        cpu->ca = (cpu->reg8 >> 7) & 0x01;                              \
        cpu->reg8 <<= 1;                                                \
        cpu->reg8 = (cpu->ca) ? cpu->reg8 | 0x01 : cpu->reg8 & (~0x01); \
        cpu->n = 0;                                                     \
        cpu->hc = 0;                                                    \
        cpu->z = (cpu->reg8) ? 0 : 1;                                   \
    }

RLC_R8(B, b)
RLC_R8(C, c)
RLC_R8(D, d)
RLC_R8(E, e)
RLC_R8(H, h)
RLC_R8(L, l)
RLC_R8(IHL, ihl)
RLC_R8(A, a)


#define RRC_R8(REG8, reg8)                                       \
    void RRC_ ## REG8 (struct machine_t *gem) {                         \
        struct cpu_t *cpu = gem->cpu;                                   \
        cpu->ca = cpu->reg8 & 0x01;                                     \
        cpu->reg8 >>= 1;                                                \
        cpu->reg8 = (cpu->ca) ? cpu->reg8 | 0x80 : cpu->reg8 & (~0x80); \
        cpu->n = 0;                                                     \
        cpu->hc = 0;                                                    \
        cpu->z = (cpu->reg8) ? 0 : 1;                                   \
    }

RRC_R8(B, b)
RRC_R8(C, c)
RRC_R8(D, d)
RRC_R8(E, e)
RRC_R8(H, h)
RRC_R8(L, l)
RRC_R8(IHL, ihl)
RRC_R8(A, a)


#define RL_R8(REG8, reg8)                                        \
    void RL_ ## REG8 (struct machine_t *gem) {                          \
        struct cpu_t *cpu = gem->cpu;                                   \
        byte temp = cpu->ca;                                            \
        cpu->ca = (cpu->reg8 >> 7) & 0x01;                              \
        cpu->reg8 <<= 1;                                                \
        cpu->reg8 = (temp) ? cpu->reg8 | 0x01 : cpu->reg8 & (~0x01);    \
        cpu->n = 0;                                                     \
        cpu->hc = 0;                                                    \
        cpu->z = (cpu->reg8) ? 0 : 1;                                   \
    }

RL_R8(B, b)
RL_R8(C, c)
RL_R8(D, d)
RL_R8(E, e)
RL_R8(H, h)
RL_R8(L, l)
RL_R8(IHL, ihl)
RL_R8(A, a)


#define RR_R8(REG8, reg8)                                        \
    void RR_ ## REG8 (struct machine_t *gem) {                          \
        struct cpu_t *cpu = gem->cpu;                                   \
        byte temp = cpu->ca;                                            \
        cpu->ca = cpu->reg8 & 0x01;                                     \
        cpu->reg8 >>= 1;                                                \
        cpu->reg8 = (temp) ? cpu->reg8 | 0x80 : cpu->reg8 & (~0x80);    \
        cpu->n = 0;                                                     \
        cpu->hc = 0;                                                    \
        cpu->z = (cpu->reg8) ? 0 : 1;                                   \
    }

RR_R8(B, b)
RR_R8(C, c)
RR_R8(D, d)
RR_R8(E, e)
RR_R8(H, h)
RR_R8(L, l)
RR_R8(IHL, ihl)
RR_R8(A, a)


#define SLA_R8(REG8, reg8)               \
    void SLA_ ## REG8 (struct machine_t *gem) { \
        struct cpu_t *cpu = gem->cpu;           \
        cpu->ca = (cpu->reg8 >> 7) & 0x01;      \
        cpu->reg8 <<= 1;                        \
        cpu->n = 0;                             \
        cpu->hc = 0;                            \
        cpu->z = (cpu->reg8) ? 0 : 1;           \
    }

SLA_R8(B, b)
SLA_R8(C, c)
SLA_R8(D, d)
SLA_R8(E, e)
SLA_R8(H, h)
SLA_R8(L, l)
SLA_R8(IHL, ihl)
SLA_R8(A, a)


#define SRA_R8(REG8, reg8)               \
    void SRA_ ## REG8 (struct machine_t *gem) { \
        struct cpu_t *cpu = gem->cpu;           \
        cpu->ca = cpu->reg8 & 0x01;             \
        cpu->reg8 >>= 1;                        \
        cpu->n = 0;                             \
        cpu->hc = 0;                            \
        cpu->z = (cpu->reg8) ? 0 : 1;           \
    }

SRA_R8(B, b)
SRA_R8(C, c)
SRA_R8(D, d)
SRA_R8(E, e)
SRA_R8(H, h)
SRA_R8(L, l)
SRA_R8(IHL, ihl)
SRA_R8(A, a)


#define SWAP_R8(REG8, reg8)                  \
    void SWAP_ ## REG8 (struct machine_t *gem) {    \
        struct cpu_t *cpu = gem->cpu;               \
        byte nibble = cpu->b & 0x0F;                \
        cpu->b >>= 4;                               \
        cpu->b &= 0x0F;                             \
        cpu->b |= nibble << 4;                      \
        cpu->ca = 0;                                \
        cpu->n = 0;                                 \
        cpu->hc = 0;                                \
        cpu->z = (cpu->reg8) ? 0 : 1;               \
    }

SWAP_R8(B, b)
SWAP_R8(C, c)
SWAP_R8(D, d)
SWAP_R8(E, e)
SWAP_R8(H, h)
SWAP_R8(L, l)
SWAP_R8(IHL, ihl)
SWAP_R8(A, a)


#define SRL_R8(REG8, reg8)               \
    void SRL_ ## REG8 (struct machine_t *gem) { \
        struct cpu_t *cpu = gem->cpu;           \
        cpu->ca = cpu->reg8 & 0x01;             \
        cpu->reg8 >>= 1;                        \
        cpu->reg8 &= ~(0x80);                   \
        cpu->n = 0;                             \
        cpu->hc = 0;                            \
        cpu->z = (cpu->reg8) ? 0 : 1;           \
    }

SRL_R8(B, b)
SRL_R8(C, c)
SRL_R8(D, d)
SRL_R8(E, e)
SRL_R8(H, h)
SRL_R8(L, l)
SRL_R8(IHL, ihl)
SRL_R8(A, a)


#define BIT_D3_R8(NUM3, REG8, reg8)                      \
    void BIT_ ## NUM3 ## _ ## REG8 (struct machine_t *gem) {    \
        struct cpu_t *cpu = gem->cpu;                           \
        cpu->z = !((cpu->reg8 >> NUM3) & 0x01);                 \
        cpu->n = 0;                                             \
        cpu->hc = 1;                                            \
    }

BIT_D3_R8(0, B, b)
BIT_D3_R8(0, C, c)
BIT_D3_R8(0, D, d)
BIT_D3_R8(0, E, e)
BIT_D3_R8(0, H, h)
BIT_D3_R8(0, L, l)
BIT_D3_R8(0, IHL, ihl)
BIT_D3_R8(0, A, a)

BIT_D3_R8(1, B, b)
BIT_D3_R8(1, C, c)
BIT_D3_R8(1, D, d)
BIT_D3_R8(1, E, e)
BIT_D3_R8(1, H, h)
BIT_D3_R8(1, L, l)
BIT_D3_R8(1, IHL, ihl)
BIT_D3_R8(1, A, a)

BIT_D3_R8(2, B, b)
BIT_D3_R8(2, C, c)
BIT_D3_R8(2, D, d)
BIT_D3_R8(2, E, e)
BIT_D3_R8(2, H, h)
BIT_D3_R8(2, L, l)
BIT_D3_R8(2, IHL, ihl)
BIT_D3_R8(2, A, a)

BIT_D3_R8(3, B, b)
BIT_D3_R8(3, C, c)
BIT_D3_R8(3, D, d)
BIT_D3_R8(3, E, e)
BIT_D3_R8(3, H, h)
BIT_D3_R8(3, L, l)
BIT_D3_R8(3, IHL, ihl)
BIT_D3_R8(3, A, a)

BIT_D3_R8(4, B, b)
BIT_D3_R8(4, C, c)
BIT_D3_R8(4, D, d)
BIT_D3_R8(4, E, e)
BIT_D3_R8(4, H, h)
BIT_D3_R8(4, L, l)
BIT_D3_R8(4, IHL, ihl)
BIT_D3_R8(4, A, a)

BIT_D3_R8(5, B, b)
BIT_D3_R8(5, C, c)
BIT_D3_R8(5, D, d)
BIT_D3_R8(5, E, e)
BIT_D3_R8(5, H, h)
BIT_D3_R8(5, L, l)
BIT_D3_R8(5, IHL, ihl)
BIT_D3_R8(5, A, a)

BIT_D3_R8(6, B, b)
BIT_D3_R8(6, C, c)
BIT_D3_R8(6, D, d)
BIT_D3_R8(6, E, e)
BIT_D3_R8(6, H, h)
BIT_D3_R8(6, L, l)
BIT_D3_R8(6, IHL, ihl)
BIT_D3_R8(6, A, a)

BIT_D3_R8(7, B, b)
BIT_D3_R8(7, C, c)
BIT_D3_R8(7, D, d)
BIT_D3_R8(7, E, e)
BIT_D3_R8(7, H, h)
BIT_D3_R8(7, L, l)
BIT_D3_R8(7, IHL, ihl)
BIT_D3_R8(7, A, a)


#define RES_D3_R8(NUM3, REG8, reg8)                             \
    void RES_ ## NUM3 ## _ ## REG8 (struct machine_t *gem) {    \
        struct cpu_t *cpu = gem->cpu;                           \
        cpu->reg8 &= ~(0x01 << NUM3);                           \
    }

RES_D3_R8(0, B, b)
RES_D3_R8(0, C, c)
RES_D3_R8(0, D, d)
RES_D3_R8(0, E, e)
RES_D3_R8(0, H, h)
RES_D3_R8(0, L, l)
RES_D3_R8(0, IHL, ihl)
RES_D3_R8(0, A, a)

RES_D3_R8(1, B, b)
RES_D3_R8(1, C, c)
RES_D3_R8(1, D, d)
RES_D3_R8(1, E, e)
RES_D3_R8(1, H, h)
RES_D3_R8(1, L, l)
RES_D3_R8(1, IHL, ihl)
RES_D3_R8(1, A, a)

RES_D3_R8(2, B, b)
RES_D3_R8(2, C, c)
RES_D3_R8(2, D, d)
RES_D3_R8(2, E, e)
RES_D3_R8(2, H, h)
RES_D3_R8(2, L, l)
RES_D3_R8(2, IHL, ihl)
RES_D3_R8(2, A, a)

RES_D3_R8(3, B, b)
RES_D3_R8(3, C, c)
RES_D3_R8(3, D, d)
RES_D3_R8(3, E, e)
RES_D3_R8(3, H, h)
RES_D3_R8(3, L, l)
RES_D3_R8(3, IHL, ihl)
RES_D3_R8(3, A, a)

RES_D3_R8(4, B, b)
RES_D3_R8(4, C, c)
RES_D3_R8(4, D, d)
RES_D3_R8(4, E, e)
RES_D3_R8(4, H, h)
RES_D3_R8(4, L, l)
RES_D3_R8(4, IHL, ihl)
RES_D3_R8(4, A, a)

RES_D3_R8(5, B, b)
RES_D3_R8(5, C, c)
RES_D3_R8(5, D, d)
RES_D3_R8(5, E, e)
RES_D3_R8(5, H, h)
RES_D3_R8(5, L, l)
RES_D3_R8(5, IHL, ihl)
RES_D3_R8(5, A, a)

RES_D3_R8(6, B, b)
RES_D3_R8(6, C, c)
RES_D3_R8(6, D, d)
RES_D3_R8(6, E, e)
RES_D3_R8(6, H, h)
RES_D3_R8(6, L, l)
RES_D3_R8(6, IHL, ihl)
RES_D3_R8(6, A, a)

RES_D3_R8(7, B, b)
RES_D3_R8(7, C, c)
RES_D3_R8(7, D, d)
RES_D3_R8(7, E, e)
RES_D3_R8(7, H, h)
RES_D3_R8(7, L, l)
RES_D3_R8(7, IHL, ihl)
RES_D3_R8(7, A, a)


#define SET_D3_R8(NUM3, REG8, reg8)                                     \
    void SET_ ## NUM3 ## _ ## REG8 (struct machine_t *gem) {            \
        struct cpu_t *cpu = gem->cpu;                                   \
        cpu->reg8 |= (0x01 << NUM3);                                    \
    }


SET_D3_R8(0, B, b)
SET_D3_R8(0, C, c)
SET_D3_R8(0, D, d)
SET_D3_R8(0, E, e)
SET_D3_R8(0, H, h)
SET_D3_R8(0, L, l)
SET_D3_R8(0, IHL, ihl)
SET_D3_R8(0, A, a)

SET_D3_R8(1, B, b)
SET_D3_R8(1, C, c)
SET_D3_R8(1, D, d)
SET_D3_R8(1, E, e)
SET_D3_R8(1, H, h)
SET_D3_R8(1, L, l)
SET_D3_R8(1, IHL, ihl)
SET_D3_R8(1, A, a)

SET_D3_R8(2, B, b)
SET_D3_R8(2, C, c)
SET_D3_R8(2, D, d)
SET_D3_R8(2, E, e)
SET_D3_R8(2, H, h)
SET_D3_R8(2, L, l)
SET_D3_R8(2, IHL, ihl)
SET_D3_R8(2, A, a)

SET_D3_R8(3, B, b)
SET_D3_R8(3, C, c)
SET_D3_R8(3, D, d)
SET_D3_R8(3, E, e)
SET_D3_R8(3, H, h)
SET_D3_R8(3, L, l)
SET_D3_R8(3, IHL, ihl)
SET_D3_R8(3, A, a)

SET_D3_R8(4, B, b)
SET_D3_R8(4, C, c)
SET_D3_R8(4, D, d)
SET_D3_R8(4, E, e)
SET_D3_R8(4, H, h)
SET_D3_R8(4, L, l)
SET_D3_R8(4, IHL, ihl)
SET_D3_R8(4, A, a)

SET_D3_R8(5, B, b)
SET_D3_R8(5, C, c)
SET_D3_R8(5, D, d)
SET_D3_R8(5, E, e)
SET_D3_R8(5, H, h)
SET_D3_R8(5, L, l)
SET_D3_R8(5, IHL, ihl)
SET_D3_R8(5, A, a)

SET_D3_R8(6, B, b)
SET_D3_R8(6, C, c)
SET_D3_R8(6, D, d)
SET_D3_R8(6, E, e)
SET_D3_R8(6, H, h)
SET_D3_R8(6, L, l)
SET_D3_R8(6, IHL, ihl)
SET_D3_R8(6, A, a)

SET_D3_R8(7, B, b)
SET_D3_R8(7, C, c)
SET_D3_R8(7, D, d)
SET_D3_R8(7, E, e)
SET_D3_R8(7, H, h)
SET_D3_R8(7, L, l)
SET_D3_R8(7, IHL, ihl)
SET_D3_R8(7, A, a)
