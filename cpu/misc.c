/*
 * misc.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include <stdlib.h>
#include "../gem.h"
#include "../cpu.h"
#include "../mmu.h"

void NOP()
{
	cpu.t += 4;
	cpu.last_t = 4;
}

void STOP()
{
	// TODO: Halt until interrupt
	cpu.t += 4;
	cpu.last_t = 4;
}

void HALT()
{
	cpu.halt = true;
	cpu.t += 4;
	cpu.last_t = 4;
}

void DI()
{
	cpu.ime = false;
	cpu.t += 4;
	cpu.last_t = 4;
}

void EI()
{
	cpu.ime = true;
	cpu.t += 4;
	cpu.last_t = 4;
}

void XX() {
	ERROR("CPU tried to execute an invalid opcode 0x%02X at 0x%04X! Exiting...", read_byte(cpu.pc), cpu.pc);
	exit(-1);
}
