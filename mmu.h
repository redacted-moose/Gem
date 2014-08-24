/*
 * mmu.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef MMU_H_
#define MMU_H_

#include <os.h>
#include "gem.h"

typedef struct mmu_t {

	// Is bios mapped?
	BOOL in_bios;

	// Memory regions
	byte bios[0x100]; // 256 byte Bios
	byte wram[0x2000]; // 8 KB of Working ram
	byte *eram; // External ram - variable
	byte zram[0x8F]; // 127 bytes of Zero-page ram
	byte *rom; // Rom cartridge
} MMU;

extern MMU mmu;

void reset_mmu();
byte read_byte(word);
word read_word(word);
void write_byte(word, byte);
void write_word(word, word);
void load_rom(const char *);

#endif /* MMU_H_ */
