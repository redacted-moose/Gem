/*
 * lsm16.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../common.h"
#include "../cpu.h"
#include "../mmu.h"
#include "lsm16.h"

#define LD_R16_D16(REG16, reg16)                        \
    void LD_ ## REG16 ## _d16 (struct machine_t *gem) { \
        struct cpu_t *cpu = gem->cpu;                   \
        cpu->reg16 = read_word(gem, cpu->pc);           \
        cpu->pc += 2;                                   \
    }

LD_R16_D16(BC, bc)
LD_R16_D16(DE, de)
LD_R16_D16(HL, hl)
LD_R16_D16(SP, sp)


#define POP_R16(REG16, reg16)                       \
    void POP_ ## REG16 (struct machine_t *gem) {    \
        struct cpu_t *cpu = gem->cpu;               \
        cpu->bc = read_word(gem, cpu->sp);          \
        cpu->sp += 2;                               \
    }

POP_R16(BC, bc)
POP_R16(DE, de)
POP_R16(HL, hl)
POP_R16(AF, af)


#define PUSH_R16(REG16, reg16)                      \
    void PUSH_ ## REG16 (struct machine_t *gem) {   \
        struct cpu_t *cpu = gem->cpu;               \
        cpu->sp -= 2;                               \
        write_word(gem, cpu->sp, cpu->bc);          \
    }

PUSH_R16(BC, bc)
PUSH_R16(DE, de)
PUSH_R16(HL, hl)
PUSH_R16(AF, af)


void LD_a16_SP(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	write_word(gem, read_word(gem, cpu->pc), cpu->sp);
}

void LD_HL_SP_r8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->hl = read_word(gem, cpu->sp + (s_byte)read_byte(gem, cpu->pc));
}

void LD_SP_HL(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->sp = cpu->hl;
}
