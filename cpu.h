/*
 * cpu.h
 *
 *  Created on: Aug 12, 2014
 *      Author: sadlercr
 */

#ifndef CPU_H_
#define CPU_H_

// #include <os.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"

// Function pointer declaring opcode functions
typedef void (*opcode)(struct machine_t *);

struct instruction_t {
    uint8_t timing1;
    uint8_t timing2;
    opcode execute;
};

struct cpu_t {

	// General purpose / Flags
	union {
		struct {
			byte    : 8;
			byte a	: 8;
		};

		struct {
			byte	: 4;
			byte ca	: 1;
			byte hc	: 1;
			byte n	: 1;
			byte z	: 1;

            byte    : 8;
		};

		word af;
	};

	union {
		struct {
			byte c;
			byte b;
		};

		word bc;
	};

	union {
		struct {
			byte e;
			byte d;
		};

		word de;
	};

	union {
		struct {
			byte l;
			byte h;
		};

		word hl;
	};

	// Special
	word sp;
	word pc;

    // Not a register, but fetched every execute cycle
    byte ihl;

	// Tick counter
    uint32_t t;

	// Last instruction's # of t-states
    uint8_t last_t;

	// Halt mode
	bool halt;

	// Interrupt master enable: enables/disables interrupts
	bool ime;

    // Which timing data to used
    bool use_timing2;

	// Array to store instruction function pointers
	/* opcode *opcodes; */
    const struct instruction_t *opcodes;

};

#define GET_REG_16(reg) cpu->(reg)
#define GET_REG_8L(reg) (byte)(cpu->(reg) & 0xFF)
#define GET_REG_8H(reg) (byte)((cpu->(reg)>> 8) & 0xFF)

#define SET_REG_16(reg, val) cpu->(reg) = (val)
#define SET_REG_8L(reg, val) cpu->(reg) = (cpu->reg & 0xFF00) | (val)
#define SET_REG_8H(reg, val) cpu->(reg) = (cpu->reg & 0x00FF) | (((word)(val)) << 8)

#define REG_A() GET_REG_8H(af)
#define REG_B() GET_REG_8H(bc)
#define REG_C() GET_REG_8L(bc)
#define REG_D() GET_REG_8H(de)
#define REG_E() GET_REG_8L(de)
#define REG_H() GET_REG_8H(hl)
#define REG_L() GET_REG_8L(hl)

#define REG_AF() GET_REG_16(af)
#define REG_BC() GET_REG_16(bc)
#define REG_DE() GET_REG_16(de)
#define REG_HL() GET_REG_16(hl)

#define GET_REG(reg) REG_ ## reg()
#define SET_REG(reg, val) REG_ ## reg(val)

#define INSTRUCTION_2(FUNC, CYCLES)             \
    {                                           \
        .timing1 = CYCLES,                      \
            .timing2 = CYCLES,                  \
            .execute = FUNC                     \
    }

#define INSTRUCTION_3(FUNC, CYCLE1, CYCLE2)     \
    {                                           \
        .timing1 = CYCLE1,                      \
            .timing2 = CYCLE2,                  \
            .execute = FUNC                     \
    }

#define INSTRUCTION(...) GET_MACRO(__VA_ARGS__, INSTRUCTION_3, INSTRUCTION_2)(__VA_ARGS__)

struct cpu_t *reset_cpu();
void destroy_cpu(struct cpu_t *);
void step_cpu(struct machine_t *);
void check_interrupts(struct machine_t *);

#endif /* CPU_H_ */
