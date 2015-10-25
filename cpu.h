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
#include "gem.h"

typedef void (*opcode)(); // Function pointer declaring opcode functions

//typedef union reg_t {
//
//	// As individual 8-bit registers
//	struct {
//		byte high;
//		byte low;
//	};
//
//	// As both combined
//	word word;
//
//} reg;

typedef struct cpu_t {

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

			byte a0 : 1;
			byte a1 : 1;
			byte a2 : 1;
			byte a3 : 1;
			byte a4 : 1;
			byte a5 : 1;
			byte a6 : 1;
			byte a7 : 1;
		};

		word af;
	};

	union {
		struct {
			byte c;
			byte b;
		};

		struct {
			byte c0 : 1;
			byte c1 : 1;
			byte c2 : 1;
			byte c3 : 1;
			byte c4 : 1;
			byte c5 : 1;
			byte c6 : 1;
			byte c7 : 1;

			byte b0 : 1;
			byte b1 : 1;
			byte b2 : 1;
			byte b3 : 1;
			byte b4 : 1;
			byte b5 : 1;
			byte b6 : 1;
			byte b7 : 1;
		};

		word bc;
	};

	union {
		struct {
			byte e;
			byte d;
		};

		struct {
			byte e0 : 1;
			byte e1 : 1;
			byte e2 : 1;
			byte e3 : 1;
			byte e4 : 1;
			byte e5 : 1;
			byte e6 : 1;
			byte e7 : 1;

			byte d0 : 1;
			byte d1 : 1;
			byte d2 : 1;
			byte d3 : 1;
			byte d4 : 1;
			byte d5 : 1;
			byte d6 : 1;
			byte d7 : 1;
		};

		word de;
	};

	union {
		struct {
			byte l;
			byte h;
		};

		struct {
			byte l0 : 1;
			byte l1 : 1;
			byte l2 : 1;
			byte l3 : 1;
			byte l4 : 1;
			byte l5 : 1;
			byte l6 : 1;
			byte l7 : 1;

			byte h0 : 1;
			byte h1 : 1;
			byte h2 : 1;
			byte h3 : 1;
			byte h4 : 1;
			byte h5 : 1;
			byte h6 : 1;
			byte h7 : 1;
		};

		word hl;
	};

	// Special
	word sp;
	word pc;



	// Tick counter
	unsigned int t;

	// Last instruction's # of t-states
	unsigned int last_t;

	// Halt mode
	bool halt;

	// Interrupt master enable: enables/disables interrupts
	bool ime;

	// Array to store instruction function pointers
	opcode *opcodes;

} CPUSTATE;

extern CPUSTATE cpu;

void reset_cpu();
void step_cpu();
void check_interrupts();

#endif /* CPU_H_ */
