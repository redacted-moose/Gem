/*
 * mmu.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

//#include <os.h>
#include <stdio.h>
#include <stdlib.h>
#include "gem.h"
#include "cpu.h"
#include "mmu.h"
#include "gpu.h"

MMU mmu = {.bios = {0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C,
		0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E, 0x11, 0x3E, 0x80, 0x32,
		0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E,
		0xFC, 0xE0, 0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A,
		0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B, 0xFE, 0x34,
		0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22,
		0x23, 0x05, 0x20, 0xF9, 0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21,
		0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
		0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0,
		0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04, 0x1E, 0x02, 0x0E, 0x0C,
		0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D,
		0x20, 0xF2, 0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62,
		0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, 0x7B, 0xE2,
		0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15,
		0x20, 0xD2, 0x05, 0x20, 0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F,
		0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
		0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED,
		0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
		0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89,
		0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
		0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC,
		0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5,
		0xB9, 0xA5, 0x42, 0x4C, 0x21, 0x04, 0x01, 0x11, 0xA8, 0x00,
		0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
		0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86,
		0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50}};

void reset_mmu()
{
	mmu.in_bios = true;
}

void load_rom(const char *path)
{
	FILE *romfile = fopen(path, "rb");
	if(romfile == NULL) {
		ERROR("File error: does not exist?\n");
		exit(1);
	}

	// Get file size
	fseek(romfile, 0L, SEEK_END);
	long size = ftell(romfile);
	fseek(romfile, 0L, SEEK_SET);
	INFO("File size is %ld\n", size);

	mmu.rom = (byte *)malloc(size * sizeof(byte));

	if(mmu.rom == NULL) {
		ERROR("Memory error: unable to allocate enough ram\n");
		exit(2);
	}

	long result = fread(mmu.rom, sizeof(byte), size, romfile);
	INFO("result is %ld\n", result);
	if(result != size) {
		ERROR("Reading error\n");
		exit(3);
	}

//	for (long i = 0; i < size; i++) {
//		mmu.rom[i] = (byte)fgetc(romfile);
//	}

	fclose(romfile);

	// Allocate ERAM based on rom values at specific addresses and size of rom
	switch(mmu.rom[0x0149]) {
	case 0: // No RAM
		break;
	case 1: // 2 KB = 1 Bank
		mmu.eram = (byte *)malloc(2048 * sizeof(byte));
		break;
	case 2: // 8 KB = 1 Bank
		mmu.eram = (byte *)malloc(8192 * sizeof(byte));
		break;
	case 3: // 32 KB = 4 Banks
		mmu.eram = (byte *)malloc(32768 * sizeof(byte));
		break;
	case 4: // 128 KB = 16 Banks
		mmu.eram = (byte *)malloc(128 * 1024 * sizeof(byte));
		break;
	}

	// Set cartridge types
	switch(mmu.rom[0x0147]) {
	case 0: // ROM Only
		break;
	case 1: // ROM + MBC1
		break;
	case 2: // ROM + MBC1 + RAM
		break;
	case 3: // ROM + MBC1 + RAM + BATT
		break;
	case 5: // ROM + MBC2
		break;
	case 6: // ROM + MBC2 + BATT
		break;
	case 8: // ROM + RAM
		break;
	case 9: // ROM + RAM + BATT
		break;
	case 0xB: // ROM + MMMO1
		break;
	case 0xC: // ROM + MMMO1 + SRAM
		break;
	case 0xD: // ROM + MMMO1 + SRAM + BATT
		break;
	case 0xF: // ROM + MBC3 + TIMER + BATT
		break;
	case 0x10: // ROM + MBC3 + TIMER + RAM + BATT
		break;
	case 0x11: // ROM + MBC3
		break;
	case 0x12: // ROM + MBC3 + RAM
		break;
	case 0x13: // ROM + MBC3 + RAM + BATT
		break;
	case 0x19: // ROM + MBC5
		break;
	case 0x1A: // ROM + MBC5 + RAM
		break;
	case 0x1B: // ROM + MBC5 + RAM + BATT
		break;
	case 0x1C: // ROM + MBC5 + RUMBLE
		break;
	case 0x1D: // ROM + MBC5 + RUMBLE + SRAM
		break;
	case 0x1E: // ROM + MBC% + RUMBLE + SRAM + BATT
		break;
	case 0x1F: // Pocket Camera
		break;
	case 0xFD: // Bandai TAMA5
		break;
	case 0xFE: // Hudson HuC-3
		break;
	case 0xFF: // Hudson HuC-1
		break;

	}
}

byte read_byte(word address)
{
	switch(address & 0xF000) {
	// Bios or Bank 0
	case 0x0000:
		if(mmu.in_bios == true) {
			if(address < 0x0100) {
//				INFO("MMU: Read BIOS at 0x%04x, got 0x%02x\n", address, mmu.bios[address]);
				return mmu.bios[address];
			} else if(cpu.pc == 0x100)
				mmu.in_bios = false;
		}
//		INFO("MMU: Read rom at 0x%04x, got 0x%02x\n", address, mmu.rom[address]);
		return mmu.rom[address];

		// Bank 0
	case 0x1000:
	case 0x2000:
	case 0x3000:
//		INFO("MMU: Read rom at 0x%04x, got 0x%02x\n", address, mmu.rom[address]);
		return mmu.rom[address];

		// Bank 1 (No bank switching yet)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
//		INFO("MMU: Read rom at 0x%04x, got 0x%02x\n", address, mmu.rom[address]);
		return mmu.rom[address];

		// Graphics VRAM
	case 0x8000:
	case 0x9000:
		// INFO("MMU: Read GPU vram at 0x%04x, got 0x%02x\n", address, gpu.vram[address & 0x1FFF]);
		return gpu.vram[address & 0x1FFF];

		// External RAM (ERAM)
	case 0xA000:
	case 0xB000:
//		INFO("MMU: Read external ram at 0x%04x, got 0x%02x\n", address, mmu.eram[address & 0x1FFF]);
		return mmu.eram[address & 0x1FFF]; // DONE?: need to wrap address into addressable range for eram

		// Working RAM (WRAM)
	case 0xC000:
	case 0xD000:
//		INFO("MMU: Read work ram at 0x%04x, got 0x%02x\n", address, mmu.wram[address & 0x1FFF]);
		return mmu.wram[address & 0x1FFF];

		// Working RAM (Shadow)
	case 0xE000:
//		INFO("MMU: Read work ram shadow at 0x%04x, got 0x%02x\n", address, mmu.wram[address & 0x1FFF]);
		return mmu.wram[address & 0x1FFF];

	case 0xF000:
		switch(address & 0x0F00) {
		case 0x000:
		case 0x100:
		case 0x200:
		case 0x300:
		case 0x400:
		case 0x500:
		case 0x600:
		case 0x700:
		case 0x800:
		case 0x900:
		case 0xA00:
		case 0xB00:
		case 0xC00:
		case 0xD00:
//			INFO("MMU: Read work ram shadow at 0x%04x, got 0x%02x\n", address, mmu.wram[address & 0x1FFF]);
			return mmu.wram[address & 0x1FFF];

		case 0xE00:
			if(address < 0xFEA0) {
//				INFO("MMU: Read GPU oam memory at 0x%04x, got 0x%02x\n", address, gpu.oam[address & 0xFF]);
				return gpu.oam[address & 0xFF];
			} else {
				return 0;
			}

		case 0xF00:
			if(address >= 0xFF80) {
//				INFO("MMU: Read zram at 0x%04x, got 0x%02x\n", address, mmu.zram[address & 0x7F]);
				return mmu.zram[address & 0x7F];
			} else {
//				INFO("MMU: Read I/O at 0x%04x, got 0x%02x\n", address);
				// I/O control -- unimplemented
//				return 0;
				switch(address & 0x00F0) {
				case 0x40:
				case 0x50:
				case 0x60:
				case 0x70:
					return read_byte_gpu(address);
				}
				return 0;
			}
		}
	}

	/* UNREACHABLE */
	__builtin_unreachable();
}

void write_byte(word address, byte val)
{
	switch(address & 0xF000) {
	case 0x8000:
	case 0x9000:
		// INFO("MMU: Wrote 0x%02x to 0x%04x (GPU vram)\n", val, address);
		gpu.vram[address & 0x1FFF] = val;
		// rendertile_gpu(address);
		break;
	case 0xF000:
		switch(address & 0x0F00) {
		case 0xF00:
			if (address >= 0xFF80) {
//				INFO("MMU: Wrote 0x%02x to 0x%04x (zram)\n", val, address);
				mmu.zram[address & 0x7F] = val;
			} else {
				switch (address & 0x00F0) {
				case 0x40: case 0x50: case 0x60: case 0x70:
					write_byte_gpu(address, val);
					break;
				}
			}
		}
	}
}

word read_word(word address)
{
	return (word)read_byte(address) | ((word)read_byte(address + 1) << 8);
}

void write_word(word address, word val)
{
	write_byte(address, (byte)(val & 0x00FF));
	write_byte(address + 1, (byte)((val >> 8) & 0x00FF));
}
