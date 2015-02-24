/*
 * cpu.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "cpu.h"
#include "cpu/opcode_table.h"

CPU cpu = { .opcodes = opcode_table };

void reset_cpu() {

}

void execute() {
	byte instruction = read_byte(cpu.pc);
//	INFO("CPU: PC = %04x; Instruction was: 0x%02x\n", cpu.pc, instruction);
	cpu.pc++;
	cpu.opcodes[instruction]();
	step_gpu();
}
