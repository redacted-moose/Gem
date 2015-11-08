/*
 * misc.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include <stdlib.h>

#include "../common.h"
#include "../cpu.h"
#include "../mmu.h"
#include "misc.h"

void NOP(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
    // Do nothing
}

void STOP(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	// TODO: Halt until interrupt
}

void HALT(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->halt = true;
}

void DI(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->ime = false;
}

void EI(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->ime = true;
}

void XX(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	ERROR("[CPU] Tried to execute an invalid opcode 0x%02X at 0x%04X! Exiting...", read_byte(gem, cpu->pc), cpu->pc);
	exit(-1);
}
