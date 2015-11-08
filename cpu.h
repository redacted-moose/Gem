/*
 * cpu.h
 *
 *  Created on: Aug 12, 2014
 *      Author: sadlercr
 */

#ifndef CPU_H_
#define CPU_H_

// #include <os.h>
#include <stdbool.h>

#include "common.h"

typedef void (*opcode)(struct machine_t *); // Function pointer declaring opcode functions

struct instruction_t {
    int timing1;
    int timing2;
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
	unsigned int t;

	// Last instruction's # of t-states
	unsigned int last_t;

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
