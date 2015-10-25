/*
 * cpu.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "cpu.h"
#include "cpu/opcode_table.h"
#include "mmu.h"
#include "gpu.h"

CPUSTATE cpu = { .opcodes = opcode_table };

void interrupt_rst(word);

void reset_cpu() {
	cpu.pc = 0;
	cpu.sp = 0;
	cpu.af = 0;
	cpu.bc = 0;
	cpu.de = 0;
	cpu.hl = 0;
	cpu.t = 0;
	cpu.last_t = 0;
	cpu.halt = 0;
	cpu.ime = 0;
}

void reset_cpu_skip_bios() {
	cpu.pc = 0x0100;
	cpu.sp = 0xFFFE;
	cpu.af = 0x0190;
	cpu.bc = 0x0013;
	cpu.de = 0x00D8;
	cpu.hl = 0x014D;
	cpu.t = 0;
	cpu.last_t = 0;
	cpu.halt = 0;
	cpu.ime = 0;
}

void step_cpu() {
	byte instruction = read_byte(cpu.pc);
	cpu.pc++;

	cpu.opcodes[instruction]();
}

CPUSTATE get_cpu() {
	return cpu;
}

void check_interrupts() {
    // Check for interrupts
	if (cpu.ime && mmu.i_enable.value && mmu.i_flag.value) {
		byte ifired = mmu.i_enable.value & mmu.i_flag.value;
		if (ifired & 0x01) {
			mmu.i_flag.vblank = 0;
			// RST 40
            interrupt_rst(0x40);
            return;
		}
		if (ifired & 0x02) {
			mmu.i_flag.lcd_stat = 0;
			// RST 48
            interrupt_rst(0x48);
            return;
		}
        if (ifired & 0x04) {
            mmu.i_flag.timer = 0;
            // RST 50
            interrupt_rst(0x50);
            return;
        }
        if (ifired & 0x08) {
            mmu.i_flag.serial = 0;
            // RST 58
            interrupt_rst(0x58);
            return;
        }
        if (ifired & 0x10) {
            mmu.i_flag.joypad = 0;
            // RST 60
            interrupt_rst(0x60);
            return;
        }
    }
}

void interrupt_rst(word location) {
    cpu.ime = 0;
    cpu.sp -= 2;
    write_word(cpu.sp, cpu.pc);
    cpu.pc = location;
    cpu.t += 20;
    cpu.last_t = 20;
}
