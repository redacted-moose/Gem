/*
 * cpu.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include <stdlib.h>

#include "common.h"
#include "gem.h"
#include "cpu.h"
#include "cpu/opcode_table.h"
#include "mmu.h"
#include "gpu.h"

void interrupt_rst(struct machine_t*, word);

struct cpu_t* reset_cpu() {
    struct cpu_t* cpu = malloc(sizeof(struct cpu_t));
    cpu->pc = 0;
    cpu->sp = 0;
    cpu->af = 0;
    cpu->bc = 0;
    cpu->de = 0;
    cpu->hl = 0;
    cpu->ihl = 0;
    cpu->t = 0;
    cpu->last_t = 0;
    cpu->halt = 0;
    cpu->ime = 0;
    cpu->opcodes = opcode_table;
    cpu->use_timing2 = false;
    INFO("CPU initialized\n");
    return cpu;
}

struct cpu_t* reset_cpu_skip_bios() {
    struct cpu_t* cpu = malloc(sizeof(struct cpu_t));
    cpu->pc = 0x0100;
    cpu->sp = 0xFFFE;
    cpu->af = 0x0190;
    cpu->bc = 0x0013;
    cpu->de = 0x00D8;
    cpu->hl = 0x014D;
    cpu->t = 0;
    cpu->last_t = 0;
    cpu->halt = 0;
    cpu->ime = 0;
    cpu->opcodes = opcode_table;
    cpu->use_timing2 = false;
    INFO("CPU initialized skipping bios\n");
    return cpu;
}

void destroy_cpu(struct cpu_t* cpu) { free(cpu); }

void step_cpu(struct machine_t* gem) {
    struct cpu_t* cpu = gem->cpu;
    struct gpu_t* gpu = gem->gpu;

    byte instruction = read_byte(gem, cpu->pc);
    INFO("Instruction is 0x%02X\n", instruction);
    cpu->pc++;

    byte ihl = cpu->ihl = read_byte(gem, cpu->hl);
    word hl = cpu->hl;

    cpu->opcodes[instruction].execute(gem);
    if (cpu->use_timing2) {
        cpu->t += cpu->opcodes[instruction].timing2;
        gpu->t += cpu->opcodes[instruction].timing2;
        cpu->last_t = cpu->opcodes[instruction].timing2;
        cpu->use_timing2 = false;
    } else {
        cpu->t += cpu->opcodes[instruction].timing1;
        gpu->t += cpu->opcodes[instruction].timing1;
        cpu->last_t = cpu->opcodes[instruction].timing1;
    }

    if (cpu->ihl != ihl) {
        write_byte(gem, hl, cpu->ihl);
    }
}

void check_interrupts(struct machine_t* gem) {
    struct cpu_t* cpu = gem->cpu;
    struct mmu_t* mmu = gem->mmu;
    // Check for interrupts
    if (cpu->ime && mmu->i_enable.value && mmu->i_flag.value) {
        byte ifired = mmu->i_enable.value & mmu->i_flag.value;
        if (ifired & 0x01) {
            mmu->i_flag.vblank = 0;
            // RST 40
            INFO("RST 40H: VBLANK interrupt\n");
            interrupt_rst(gem, 0x40);
            return;
        }
        if (ifired & 0x02) {
            mmu->i_flag.lcd_stat = 0;
            // RST 48
            INFO("RST 48H: LCD Status interrupt\n");
            interrupt_rst(gem, 0x48);
            return;
        }
        if (ifired & 0x04) {
            mmu->i_flag.timer = 0;
            // RST 50
            INFO("RST 50H: Timer interrupt\n");
            interrupt_rst(gem, 0x50);
            return;
        }
        if (ifired & 0x08) {
            mmu->i_flag.serial = 0;
            // RST 58
            INFO("RST 58H: Serial interrupt\n");
            interrupt_rst(gem, 0x58);
            return;
        }
        if (ifired & 0x10) {
            mmu->i_flag.joypad = 0;
            // RST 60
            INFO("RST 60H: Joypad interrupt\n");
            interrupt_rst(gem, 0x60);
            return;
        }
    }
}

void interrupt_rst(struct machine_t* gem, word location) {
    struct cpu_t* cpu = gem->cpu;
    cpu->ime = 0;
    cpu->sp -= 2;
    write_word(gem, cpu->sp, cpu->pc);
    cpu->pc = location;
    cpu->t += 20;
    cpu->last_t = 20;
}
