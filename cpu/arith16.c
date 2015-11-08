/*
 * arith16.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../cpu.h"
#include "../mmu.h"
#include "arith16.h"


#define INC_R16(REG16, reg16)                \
    void INC_ ## REG16 (struct machine_t *gem) {    \
        struct cpu_t *cpu = gem->cpu;               \
        cpu->reg16++;                               \
    }

INC_R16(BC, bc)
INC_R16(DE, de)
INC_R16(HL, hl)
INC_R16(SP, sp)


#define DEC_R16(REG16, reg16)                \
    void DEC_ ## REG16 (struct machine_t *gem) {    \
        struct cpu_t *cpu = gem->cpu;               \
        cpu->reg16--;                               \
    }

DEC_R16(BC, bc)
DEC_R16(DE, de)
DEC_R16(HL, hl)
DEC_R16(SP, sp)


#define ADD_HL_R16(REG16, reg16)                             \
    void ADD_HL_ ## REG16 (struct machine_t *gem) {                 \
        struct cpu_t *cpu = gem->cpu;                               \
        word before = cpu->hl;                                      \
        cpu->hl += cpu->reg16;                                      \
        cpu->n = 0;                                                 \
        cpu->hc = ((before & 0x0FFF) > (cpu->hl & 0x0FFF))? 1 : 0;  \
        cpu->ca = (before > cpu->hl)? 1 : 0;                        \
    }

ADD_HL_R16(BC, bc)
ADD_HL_R16(DE, de)
ADD_HL_R16(HL, hl)
ADD_HL_R16(SP, sp)


void ADD_SP_r8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	word before = cpu->sp;
	cpu->sp += read_byte(gem, cpu->pc);
	cpu->pc++;
	cpu->z = 0;
	cpu->n = 0;
	cpu->hc = ((before & 0x0FFF) > (cpu->hl & 0x0FFF))? 1 : 0;
	cpu->ca = (before > cpu->hl)? 1 : 0;
}
