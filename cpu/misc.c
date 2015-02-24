/*
 * misc.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../gem.h"
#include "../cpu.h"

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
	ERROR("CPU tried to execute an invalid opcode! Exiting...");
	exit(-1);
}
