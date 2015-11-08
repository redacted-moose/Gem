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
#include "input.h"

const byte bios_array[0x100] = {0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C,
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
                                0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50};

struct mmu_t *reset_mmu() {
    struct mmu_t *mmu = malloc(sizeof(struct mmu_t));
	mmu->in_bios = true;
    mmu->bios = bios_array;
    mmu->dma = 0xFF;
    INFO("MMU initialized\n");
    return mmu;
}

void destroy_mmu(struct mmu_t *mmu) {
    free(mmu);
}

void load_rom(struct mmu_t *mmu, const char *path) {
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

	mmu->rom = (byte *)malloc(size * sizeof(byte));

	if(mmu->rom == NULL) {
		ERROR("Memory error: unable to allocate enough ram\n");
		exit(2);
	}

	long result = fread(mmu->rom, sizeof(byte), size, romfile);
	/* INFO("Result is %ld\n", result); */
	if(result != size) {
		ERROR("Reading error\n");
		exit(3);
	}

	fclose(romfile);

	// Allocate ERAM based on rom values at specific addresses and size of rom
    INFO("Internal RAM: ");
	switch(mmu->rom[0x0149]) {
	case 0: // No RAM
        // Optionally allocate 8 KB external ram
        PRINT("No internal RAM\n");
		break;
	case 1: // 2 KB = 1 Bank
        PRINT("1 Bank (2 KB)\n");
		mmu->eram = (byte *)malloc(2048 * sizeof(byte));
		break;
	case 2: // 8 KB = 1 Bank
        PRINT("1 Bank (8 KB)\n");
		mmu->eram = (byte *)malloc(8192 * sizeof(byte));
		break;
	case 3: // 32 KB = 4 Banks
        PRINT("4 Banks (32 KB)\n");
		mmu->eram = (byte *)malloc(32768 * sizeof(byte));
		break;
	case 4: // 128 KB = 16 Banks
        PRINT("16 Banks (128 KB)\n");
		mmu->eram = (byte *)malloc(128 * 1024 * sizeof(byte));
		break;
	}

	// Set cartridge types
    mmu->rom_type = mmu->rom[0x0147];
    #ifdef DEBUG
    INFO("Rom type: ");
	switch(mmu->rom_type) {
	case ROM_ONLY: // ROM Only
        PRINT("ROM only\n");
		break;
	case MBC1: // ROM + MBC1
        PRINT("MBC1\n");
		break;
	case MBC1_RAM: // ROM + MBC1 + RAM
        PRINT("MBC1 + RAM\n");
		break;
	case MBC1_RAM_BATT: // ROM + MBC1 + RAM + BATT
        PRINT("MBC1 + RAM + BATT\n");
		break;
	case MBC2: // ROM + MBC2
        PRINT("MBC2\n");
		break;
	case MBC2_BATT: // ROM + MBC2 + BATT
        PRINT("MBC2 + BATT\n");
		break;
	case RAM: // ROM + RAM
        PRINT("RAM\n");
		break;
	case RAM_BATT: // ROM + RAM + BATT
        PRINT("RAM + BATT\n");
		break;
	case MMMO1: // ROM + MMMO1
        PRINT("MMMO1\n");
		break;
	case MMMO1_SRAM: // ROM + MMMO1 + SRAM
        PRINT("MMMO1 + SRAM\n");
		break;
	case 0xD: // ROM + MMMO1 + SRAM + BATT
        PRINT("MMMO1 + SRAM + BATT\n");
		break;
	case 0xF: // ROM + MBC3 + TIMER + BATT
        PRINT("MBC3 + TIMER + BATT\n");
		break;
	case 0x10: // ROM + MBC3 + TIMER + RAM + BATT
        PRINT("MBC3 + TIMER + RAM + BATT\n");
		break;
	case 0x11: // ROM + MBC3
        PRINT("MBC3\n");
		break;
	case 0x12: // ROM + MBC3 + RAM
        PRINT("MBC3 + RAM\n");
		break;
	case 0x13: // ROM + MBC3 + RAM + BATT
        PRINT("MBC3 + RAM + BATT\n");
		break;
	case 0x19: // ROM + MBC5
        PRINT("MBC5\n");
		break;
	case 0x1A: // ROM + MBC5 + RAM
        PRINT("MBC5 + RAM\n");
		break;
	case 0x1B: // ROM + MBC5 + RAM + BATT
        PRINT("MBC5 + RAM + BATT\n");
		break;
	case 0x1C: // ROM + MBC5 + RUMBLE
        PRINT("MBC5 + RUMBLE\n");
		break;
	case 0x1D: // ROM + MBC5 + RUMBLE + SRAM
        PRINT("MBC5 + RUMBLE + SRAM\n");
		break;
	case 0x1E: // ROM + MBC% + RUMBLE + SRAM + BATT
        PRINT("MBC5 + RUMBLE + SRAM + BATT\n");
		break;
	case 0xFC: // Pocket Camera
        PRINT("Pocket Camera\n");
		break;
	case 0xFD: // Bandai TAMA5
        PRINT("Bandai TAMA5\n");
		break;
	case 0xFE: // Hudson HuC-3
        PRINT("Hudson HuC-3\n");
		break;
	case 0xFF: // Hudson HuC-1
        PRINT("Hudson HuC-1\n");
		break;
    default:
        PRINT("Unknown 0x%02X", mmu->rom_type);
	}
    #endif
}

byte read_byte(struct machine_t *gem, word address) {
    struct mmu_t *mmu = gem->mmu;
    struct gpu_t *gpu = gem->gpu;
    struct cpu_t *cpu = gem->cpu;

	switch(address & 0xF000) {
	// Bios or Bank 0
	case 0x0000:
		if(mmu->in_bios) {
			if(address < 0x0100) {
				return mmu->bios[address];
			} else if(cpu->pc == 0x0100) {
				mmu->in_bios = false;
            }
		}

        return (mmu->rom != NULL) ? mmu->rom[address] : 0xFF;

    // Bank 0
	case 0x1000:
	case 0x2000:
	case 0x3000:
        return (mmu->rom != NULL) ? mmu->rom[address] : 0xFF;

    // Bank 1 (No bank switching yet)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return (mmu->rom != NULL) ? mmu->rom[address] : 0xFF;

    // Graphics VRAM
	case 0x8000:
	case 0x9000:
		return gpu->vram[address & 0x1FFF];

    // External RAM (ERAM)
	case 0xA000:
	case 0xB000:
        return (mmu->eram != NULL) ? mmu->eram[address & 0x1FFF] : 0xFF;

    // Working RAM (WRAM)
	case 0xC000:
	case 0xD000:
		return mmu->wram[address & 0x1FFF];

    // Working RAM (Shadow)
	case 0xE000:
		return mmu->wram[address & 0x1FFF];

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
			return mmu->wram[address & 0x1FFF];

		case 0xE00:
			if(address < 0xFEA0) {
				return gpu->oam[address & 0x9F];
			} else {
                WARN("Attempted read from 0x%04X.\n", address);
				return 0xFF;
			}

		case 0xF00:
            switch (address & 0x00F0) {
            case 0x00:
                switch (address & 0x000F) {
                case 0x0:
                    return read_byte_key(gem, address);
                case 0x1:
                case 0x2:
                case 0x4:
                case 0x5:
                case 0x6:
                case 0x7:
                    WARN("Attempted read from 0x%04X.\n", address);
                    return 0xFF;
                case 0xF:
                    return mmu->i_flag.value;
                default:
                    WARN("Attempted read from 0x%04X.\n", address);
                    return 0xFF;
                }
            case 0x10:
            case 0x20:
            case 0x30:
                WARN("Attempted read from 0x%04X.\n", address);
                return 0xFF;
            case 0x40:
                switch (address & 0x000F) {
                case 0x6:
                case 0xC:
                case 0xD:
                case 0xE:
                case 0xF:
                    WARN("Attempted read from 0x%04X.\n", address);
                    return 0xFF;
                default:
                    return read_byte_gpu(gpu, address);
                }
            case 0x50:
                switch (address & 0x000F) {
                case 0x0:
                    return !mmu->in_bios;
                default:
                    WARN("Attempted read from 0x%04X.\n", address);
                    return 0xFF;
                }
            case 0x60:
            case 0x70:
                WARN("Attempted read from 0x%04X.\n", address);
                return 0xFF;
            case 0x80:
            case 0x90:
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
                return mmu->zram[address & 0x7F];
            case 0xF0:
                switch (address & 0x000F) {
                case 0xF:
                    return mmu->i_enable.value;
                default:
                    return mmu->zram[address & 0x7F];
                }
            default:
                WARN("Attempted read from 0x%04X.\n", address);
                return 0xFF;
            }
        default:
            WARN("Attempted read from 0x%04X.\n", address);
            return 0xFF;
		}
    default:
        WARN("Attempted read from 0x%04X.\n", address);
        return 0xFF;
	}
}

void write_byte(struct machine_t *gem, word address, byte val) {
    struct mmu_t *mmu = gem->mmu;
    struct gpu_t *gpu = gem->gpu;

	switch(address & 0xF000) {
	case 0x8000:
	case 0x9000:
		gpu->vram[address & 0x1FFF] = val;
		break;
    case 0xA000:
    case 0xB000:
        if (mmu->eram != NULL) {
            mmu->eram[address & 0x1FFF] = val;
        } else {
            WARN("Attempted to write 0x%02X to 0x%04X\n", val, address);
        }
        break;
    case 0xC000:
    case 0xD000:
        mmu->wram[address & 0x1FFF] = val;
        break;
    case 0xE000:
        mmu->wram[address & 0x1FFF] = val;
        break;
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
            mmu->wram[address & 0x1FFF] = val;
            break;
		case 0xE00:
			if (address < 0xFEA0) {
				gpu->oam[address & 0x9F] = val;
			} else {
                WARN("Attempted to write 0x%02X to 0x%04X\n", val, address);
            }
			break;
        case 0xF00:
            switch (address & 0x00F0) {
            case 0x00:
                switch (address & 0x000F) {
                case 0x0:
                    write_byte_key(gem, address, val);
                    break;
                case 0xF:
                    mmu->i_flag.value = val;
                    break;
                default:
                    WARN("Attempted to write 0x%02X to 0x%04X\n", val, address);
                    break;
                }
                break;
            case 0x40:
                switch (address & 0x000F) {
                case 0x6:
                    INFO("DMA requested from 0x%02X00-0x%02X9F to 0xFE00-0xFE9F\n", val, val);
                    mmu->dma = val;
                    mmu->dma_offset = 0;
                    break;
                case 0xC:
                case 0xD:
                case 0xE:
                case 0xF:
                    WARN("Attempted to write 0x%02X to 0x%04X\n", val, address);
                    break;
                default:
                    write_byte_gpu(gpu, address, val);
                    break;
                }
                break;
            case 0x50:
                switch (address & 0x000F) {
                case 0x0:
                    if (mmu->in_bios) {
                        mmu->in_bios = !val;
                    } else {
                        WARN("Attempted to write to read-only register 0xFF50\n");
                    }
                    break;
                default:
                    WARN("Attempted to write 0x%02X to 0x%04X\n", val, address);
                    break;
                }
                break;
            case 0x80:
            case 0x90:
            case 0xA0:
            case 0xB0:
            case 0xC0:
            case 0xD0:
            case 0xE0:
                mmu->zram[address & 0x7F] = val;
                break;
            case 0xF0:
                switch (address & 0xF) {
                case 0xF:
                    mmu->i_enable.value = val;
                    break;
                default:
                    mmu->zram[address & 0x7F] = val;
                    break;
                }
                break;
            default:
                WARN("Attempted to write 0x%02X to 0x%04X\n", val, address);
                break;
            }
            break;
        default:
            WARN("Attempted to write 0x%02X to 0x%04X\n", val, address);
            break;
		}
        break;
    default:
        WARN("Attempted to write 0x%02X to 0x%04X\n", val, address);
        break;
	}
}

word read_word(struct machine_t *gem, word address) {
	return (word)read_byte(gem, address) | ((word)read_byte(gem, address + 1) << 8);
}

void write_word(struct machine_t *gem, word address, word val) {
	write_byte(gem, address, (byte)(val & 0x00FF));
	write_byte(gem, address + 1, (byte)((val >> 8) & 0x00FF));
}

void do_dma(struct machine_t *gem) {
    // Need to disable other areas of memory when this is happening.
    struct mmu_t *mmu = gem->mmu;
    if (mmu->dma <= 0xF1 && mmu->dma_offset <= 0x9F) {
        write_byte(gem, 0xFE00 + mmu->dma_offset, read_byte(gem, ((word)mmu->dma << 8) + mmu->dma_offset));
        mmu->dma_offset++;
    }
}
