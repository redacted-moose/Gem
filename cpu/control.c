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


#define JR_COND_R8(COND, cond)                   \
    void JR ## COND ## _r8 (struct machine_t *gem) {    \
        struct cpu_t *cpu = gem->cpu;                   \
        if (cond) {                                     \
            cpu->pc += (s_byte)read_byte(gem, cpu->pc); \
        } else {                                        \
            cpu->use_timing2 = true;                    \
        }                                               \
        cpu->pc++;                                      \
    }

JR_COND_R8(, true)
JR_COND_R8(_NZ, !cpu->z)
JR_COND_R8(_Z, cpu->z)
JR_COND_R8(_NC, !cpu->ca)
JR_COND_R8(_C, cpu->ca)


#define RET_COND(COND, cond)                        \
    void RET ## COND (struct machine_t *gem) {             \
        struct cpu_t *cpu = gem->cpu;                      \
        if (cond) {                                        \
            cpu->pc = read_word(gem, cpu->sp);             \
            cpu->sp += 2;                                  \
        } else {                                           \
            cpu->use_timing2 = true;                       \
        }                                                  \
    }

RET_COND(, true)
RET_COND(_NZ, !cpu->z)
RET_COND(_Z, cpu->z)
RET_COND(_NC, !cpu->ca)
RET_COND(_C, cpu->ca)

void RETI(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->pc = read_word(gem, cpu->sp);
    cpu->sp += 2;
	cpu->ime = true;
}


#define JP_COND_A16(COND, cond)                  \
    void JP ## COND ## _a16 (struct machine_t *gem) {   \
        struct cpu_t *cpu = gem->cpu;                   \
        if (cond) {                                     \
            cpu->pc = read_word(gem, cpu->pc);          \
        } else {                                        \
            cpu->pc += 2;                               \
            cpu->use_timing2 = true;                    \
        }                                               \
    }

JP_COND_A16(, true)
JP_COND_A16(_NZ, !cpu->z)
JP_COND_A16(_Z, cpu->z)
JP_COND_A16(_NC, !cpu->ca)
JP_COND_A16(_C, cpu->ca)

void JP_HL(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
    cpu->pc = cpu->hl;
}


#define CALL_COND_A16(COND, cond)                     \
    void CALL ## COND ## _a16 (struct machine_t *gem) {      \
        struct cpu_t *cpu = gem->cpu;                        \
        if (cond) {                                          \
            cpu->sp -= 2;                                    \
            write_word(gem, cpu->sp, cpu->pc + 2);           \
            cpu->pc = read_word(gem, cpu->pc);               \
        } else {                                             \
            cpu->pc += 2;                                    \
            cpu->use_timing2 = true;                         \
        }                                                    \
    }

CALL_COND_A16(, true)
CALL_COND_A16(_NZ, !cpu->z)
CALL_COND_A16(_Z, cpu->z)
CALL_COND_A16(_NC, !cpu->ca)
CALL_COND_A16(_C, cpu->ca)


#define RST_A16(A16, a16)                \
    void RST ## A16 (struct machine_t *gem) {   \
        struct cpu_t *cpu = gem->cpu;           \
        cpu->sp -= 2;                           \
        write_word(gem, cpu->sp, cpu->pc);      \
        cpu->pc = a16;                          \
    }

RST_A16(_00H, 0x0000)
RST_A16(_08H, 0x0008)
RST_A16(_10H, 0x0010)
RST_A16(_18H, 0x0018)
RST_A16(_20H, 0x0020)
RST_A16(_28H, 0x0028)
RST_A16(_30H, 0x0030)
RST_A16(_38H, 0x0038)
