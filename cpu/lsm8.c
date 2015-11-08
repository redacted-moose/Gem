/*
 * lsm8.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../cpu.h"
#include "../mmu.h"
#include "lsm8.h"

#define LD_IR16_A(REG16, reg16)                          \
    void LD_ ## REG16 ## _A (struct machine_t *gem) {           \
        struct cpu_t *cpu = gem->cpu;                           \
        write_byte(gem, cpu->reg16, cpu->a);                    \
    }

LD_IR16_A(BC, bc)
LD_IR16_A(DE, de)


#define LD_R8_ID8(REG8, reg8)                    \
    void LD_ ## REG8 ## _d8 (struct machine_t *gem) {   \
        struct cpu_t *cpu = gem->cpu;                   \
        cpu->reg8 = read_byte(gem, cpu->pc);            \
        cpu->pc++;                                      \
    }

LD_R8_ID8(B, b)
LD_R8_ID8(C, c)
LD_R8_ID8(D, d)
LD_R8_ID8(E, e)
LD_R8_ID8(H, h)
LD_R8_ID8(L, l)
LD_R8_ID8(A, a)


#define LD_A_IR16(REG16, reg16)                          \
    void LD_A_ ## REG16 (struct machine_t *gem) {               \
        struct cpu_t *cpu = gem->cpu;                           \
        cpu->a = read_byte(gem, cpu->reg16);                    \
    }

LD_A_IR16(BC, bc)
LD_A_IR16(DE, de)


#define LD_R8_R8(REG8, reg8, REG82, reg82)               \
    void LD_ ## REG8 ## _ ## REG82 (struct machine_t *gem) {    \
        struct cpu_t *cpu = gem->cpu;                           \
        cpu->reg8 = cpu->reg82;                                 \
    }

LD_R8_R8(B, b, B, b)
LD_R8_R8(B, b, C, c)
LD_R8_R8(B, b, D, d)
LD_R8_R8(B, b, E, e)
LD_R8_R8(B, b, H, h)
LD_R8_R8(B, b, L, l)
LD_R8_R8(B, b, IHL, ihl)
LD_R8_R8(B, b, A, a)

LD_R8_R8(C, c, B, b)
LD_R8_R8(C, c, C, c)
LD_R8_R8(C, c, D, d)
LD_R8_R8(C, c, E, e)
LD_R8_R8(C, c, H, h)
LD_R8_R8(C, c, L, l)
LD_R8_R8(C, c, IHL, ihl)
LD_R8_R8(C, c, A, a)

LD_R8_R8(D, d, B, b)
LD_R8_R8(D, d, C, c)
LD_R8_R8(D, d, D, d)
LD_R8_R8(D, d, E, e)
LD_R8_R8(D, d, H, h)
LD_R8_R8(D, d, L, l)
LD_R8_R8(D, d, IHL, ihl)
LD_R8_R8(D, d, A, a)

LD_R8_R8(E, e, B, b)
LD_R8_R8(E, e, C, c)
LD_R8_R8(E, e, D, d)
LD_R8_R8(E, e, E, e)
LD_R8_R8(E, e, H, h)
LD_R8_R8(E, e, L, l)
LD_R8_R8(E, e, IHL, ihl)
LD_R8_R8(E, e, A, a)

LD_R8_R8(H, h, B, b)
LD_R8_R8(H, h, C, c)
LD_R8_R8(H, h, D, d)
LD_R8_R8(H, h, E, e)
LD_R8_R8(H, h, H, h)
LD_R8_R8(H, h, L, l)
LD_R8_R8(H, h, IHL, ihl)
LD_R8_R8(H, h, A, a)

LD_R8_R8(L, l, B, b)
LD_R8_R8(L, l, C, c)
LD_R8_R8(L, l, D, d)
LD_R8_R8(L, l, E, e)
LD_R8_R8(L, l, H, h)
LD_R8_R8(L, l, L, l)
LD_R8_R8(L, l, IHL, ihl)
LD_R8_R8(L, l, A, a)

LD_R8_R8(IHL, ihl, B, b)
LD_R8_R8(IHL, ihl, C, c)
LD_R8_R8(IHL, ihl, D, d)
LD_R8_R8(IHL, ihl, E, e)
LD_R8_R8(IHL, ihl, H, h)
LD_R8_R8(IHL, ihl, L, l)
LD_R8_R8(IHL, ihl, A, a)

LD_R8_R8(A, a, B, b)
LD_R8_R8(A, a, C, c)
LD_R8_R8(A, a, D, d)
LD_R8_R8(A, a, E, e)
LD_R8_R8(A, a, H, h)
LD_R8_R8(A, a, L, l)
LD_R8_R8(A, a, IHL, ihl)
LD_R8_R8(A, a, A, a)


void LDI_HL_A(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	/* write_byte(gem, cpu->hl, cpu->a); */
    cpu->ihl = cpu->a;
	cpu->hl++;
}


void LDI_A_HL(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	/* cpu->a = read_byte(gem, cpu->hl); */
    cpu->a = cpu->ihl;
	cpu->hl++;
}

void LDD_HL_A(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	/* write_byte(gem, cpu->hl, cpu->a); */
    cpu->ihl = cpu->a;
	cpu->hl--;
}

void LD_IHL_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	/* write_byte(gem, cpu->hl, read_byte(gem, cpu->pc)); */
    cpu->ihl = read_byte(gem, cpu->pc);
	cpu->pc++;
}

void LDD_A_HL(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	/* cpu->a = read_byte(gem, cpu->hl); */
    cpu->a = cpu->ihl;
	cpu->hl--;
}

void LDH_a8_A(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	write_byte(gem, 0xFF00 + read_byte(gem, cpu->pc), cpu->a);
	cpu->pc++;
}

void LD_IC_A(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	write_byte(gem, 0xFF00 + cpu->c, cpu->a);
}

void LD_a16_A(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	write_byte(gem, read_word(gem, cpu->pc), cpu->a);
	cpu->pc += 2;
}

void LDH_A_a8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->a = read_byte(gem, 0xFF00 + read_byte(gem, cpu->pc));
	cpu->pc++;
}

void LD_A_IC(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->a = read_byte(gem, 0xFF00 + cpu->c);
}

void LD_A_a16(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->a = read_byte(gem, read_word(gem, cpu->pc));
	cpu->pc += 2;
}

