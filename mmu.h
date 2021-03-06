/*
 * mmu.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef MMU_H_
#define MMU_H_

//#include <os.h>
#include <stdbool.h>
#include "common.h"

typedef enum cartridge_t {
    ROM_ONLY = 0,
    MBC1 = 1,
    MBC1_RAM = 2,
    MBC1_RAM_BATT = 3,
    MBC2 = 5,
    MBC2_BATT = 6,
    RAM = 8,
    RAM_BATT = 9,
    MMMO1 = 0xB,
    MMMO1_SRAM = 0xC,
    MMMO1_SRAM_BATT = 0xD,
    MBC3_TIMER_BATT = 0xF,
    MBC3_TIMER_RAM_BATT = 0x10,
    MBC3 = 0x11,
    MBC3_RAM = 0x12,
    MBC3_RAM_BATT = 0x13,
    MBC5 = 0x19,
    MBC5_RAM = 0x1A,
    MBC5_RAM_BATT = 0x1B,
    MBC5_RUMBLE = 0x1C,
    MBC5_RUMBLE_SRAM = 0x1D,
    MBC5_RUMBLE_SRAM_BATT = 0x1E,
    POCKET_CAMERA = 0xFC,
    BANDAI_TAMA5 = 0xFD,
    HUDSON_HUC_3 = 0xFE,
    HUDSON_HUC_1 = 0xFF
} CARTRIDGE;

struct mmu_t {

	// Is bios mapped?
	bool in_bios;

	// Memory regions
	const byte *bios;// 256 byte Bios
	byte wram[0x2000]; // 8 KB of Working ram
	byte *eram; // External ram - variable
	byte zram[0x7F]; // 127 bytes of Zero-page ram
	byte *rom; // Rom cartridge

    union {
        struct {
            byte vblank : 1;
            byte lcd_stat : 1;
            byte timer : 1;
            byte serial : 1;
            byte joypad : 1;
            byte : 3;
        };

        byte value;
    } i_enable;

    union {
        struct {
            byte vblank : 1;
            byte lcd_stat : 1;
            byte timer : 1;
            byte serial : 1;
            byte joypad : 1;
            byte : 3;
        };

        byte value;
    } i_flag;

    CARTRIDGE rom_type;

    byte dma;
    byte dma_offset;

};

extern const byte bios_array[0x100];

struct mmu_t *reset_mmu();
void destroy_mmu(struct mmu_t *);
byte read_byte(struct machine_t *, word);
word read_word(struct machine_t *, word);
void write_byte(struct machine_t *,word, byte);
void write_word(struct machine_t *, word, word);
void load_rom(struct mmu_t *, const char *);
void do_dma(struct machine_t *);

#endif /* MMU_H_ */
