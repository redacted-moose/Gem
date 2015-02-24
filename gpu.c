/*
 * gpu.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

//#include <os.h>
#include "gpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "cpu.h"
#include "mmu.h"

GPU gpu;

SDL_Surface *screen;

void renderscan_gpu();
void renderscreen_gpu();

void reset_gpu() {
	gpu.mode = 0;
	gpu.curline = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(289, 144, 8, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set 160x144x8 video mode: %s\n",
				SDL_GetError());
		exit(1);
	}

	SDL_WM_SetCaption("Gem - a Gameboy Emulator", NULL);
}

//byte screen[160 * 144 / 2];

//int color_map[4] = { 0xF, // WHITE
//		0xA, 0x5, 0x0 // BLACK
//		};

Uint32 color_map[4] = {
		0xFFFFFFFF,
		0xAAAAAAAA,
		0x55555555,
		0x00000000
};

// This is sketchy...
void setPixel(int x, int y, Uint32 color) {
//	unsigned char* p = (unsigned char*)(screen
//			+ ((x >> 1) + (y << 7) + (y << 5)));
//	*p = (x & 1) ? ((*p & 0xF0) | color) : ((*p & 0x0F) | (color << 4));

	int bpp = screen->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *) screen->pixels + y * screen->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = color;
		break;

	case 2:
		*(Uint16 *) p = color;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (color >> 16) & 0xff;
			p[1] = (color >> 8) & 0xff;
			p[2] = color & 0xff;
		} else {
			p[0] = color & 0xff;
			p[1] = (color >> 8) & 0xff;
			p[2] = (color >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *) p = color;
		break;
	}
}

void step_gpu() {
	switch (gpu.mode) {
	case OAM_READ:
		if (cpu.t >= 80) {
			cpu.t = 0;
//			INFO("GPU: Entering VRAM_READ...\n");
			gpu.mode = VRAM_READ;
		}
		break;

	case VRAM_READ:
		if (cpu.t >= 172) {
			cpu.t = 0;
//			INFO("GPU: Entering HBLANK...\n");
			gpu.mode = HBLANK;

			renderscan_gpu();
		}
		break;

	case HBLANK:
		if (cpu.t >= 204) {
			cpu.t = 0;
			gpu.curline++;
			if (gpu.curline == 143) {
//				INFO("Entering VBLANK...\n");
				gpu.mode = VBLANK;
				// plot to screen
				renderscreen_gpu();
			} else {
//				INFO("GPU: Entering OAM_READ...\n");
				gpu.mode = OAM_READ;
			}
		}
		break;

	case VBLANK:
		if (cpu.t >= 456) {
			cpu.t = 0;
			gpu.curline++;

			if (gpu.curline > 153) {
//				INFO("GPU: Entering OAM_READ...\n");
				gpu.mode = OAM_READ;
				gpu.curline = 0;
			}
		}
		break;
	}
}

void renderscan_gpu() {
//	INFO("GPU: Rendering scanline %d\n", gpu.curline);
//	byte *map = (gpu.bg_tilemap) ? gpu.map0 : gpu.map1;
	byte mapoffs = (gpu.bg_tilemap) ? 0x1C00 : 0x1800;
	mapoffs += (gpu.curline + gpu.scroll_y) / 8;
	byte lineoffs = gpu.scroll_x / 8;
	byte y = (gpu.curline + gpu.scroll_y) % 8;
	byte x = gpu.scroll_x % 8;
//	byte screenoffs = gpu.curline * 160;
	byte tileoffset = read_byte(0x8000 + mapoffs + lineoffs);
//	tile curTile;
	word curTile[8];

	if (gpu.bg_tileset) { // Use signed tiles
//		curTile = gpu.tileset[0x100 + (s_byte) tileoffset];
		// FIXME: Need to find a way to convert memory location to tile - maybe a wrapper routine
//		curTile = (tile)(gpu.vram[0x100 + (s_byte) tileoffset]);
		memcpy(&curTile, gpu.vram + 0x100 + (s_byte) tileoffset, sizeof(curTile));
	} else { // Use unsigned tiles
//		curTile = gpu.tileset[tileoffset];
//		curTile = (tile)(gpu.vram[tileoffset]);
		memcpy(&curTile, gpu.vram + tileoffset, sizeof(curTile));
	}

	for(int i = 0; i < 160; i++) {
		word curRow = curTile[y];
		byte color_index = (byte)(((curRow >> (8 + x)) & 0x1) << 1) | (byte)((curRow >> x) & 0x1);
		byte color = (gpu.background_palette >> (2 * color_index)) & 0x03;

		if (SDL_MUSTLOCK(screen)) {
			if (SDL_LockSurface(screen) < 0) {
				fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
				return;
			}
		}

		setPixel(i, gpu.curline, color_map[color]);

		if (SDL_MUSTLOCK(screen)) {
			SDL_UnlockSurface(screen);
		}

		x++;
		if (x == 8) {
			x = 0;
			lineoffs = (lineoffs + 1) % 32;
//			tileoffset = map[mapoffs + lineoffs];
			tileoffset = read_byte(0x8000 + mapoffs + lineoffs);
			if (gpu.bg_tileset) { // Use signed tiles
//				curTile = gpu.tileset[0x100 + (s_byte) tileoffset];
//				curTile = (tile)(gpu.vram* + 0x100 + (s_byte) tileoffset);
				memcpy(&curTile, gpu.vram + 0x100 + (s_byte) tileoffset, sizeof(curTile));
			} else { // Use unsigned tiles
//				curTile = gpu.tileset[tileoffset];
//				curTile = (tile)(gpu.vram + tileoffset);
				memcpy(&curTile, gpu.vram + tileoffset, sizeof(curTile));
			}
		}
	}
//	for (int i = 0; i < 160; i++) {
//		byte color;
//		row curRow = curTile.rows[y];
//		byte color_index;
//		switch (x) {
//		case 0:
//			color_index = curRow.a0 | (curRow.b0 << 1);
//			break;
//		case 1:
//			color_index = curRow.a1 | (curRow.b1 << 1);
//			break;
//		case 2:
//			color_index = curRow.a2 | (curRow.b2 << 1);
//			break;
//		case 3:
//			color_index = curRow.a3 | (curRow.b3 << 1);
//			break;
//		case 4:
//			color_index = curRow.a4 | (curRow.b4 << 1);
//			break;
//		case 5:
//			color_index = curRow.a5 | (curRow.b5 << 1);
//			break;
//		case 6:
//			color_index = curRow.a6 | (curRow.b6 << 1);
//			break;
//		case 7:
//			color_index = curRow.a7 | (curRow.b7 << 1);
//			break;
//		}
//		color = (gpu.background_palette >> (2 * color_index)) & 0x03;
//
//		if (SDL_MUSTLOCK(screen)) {
//			if (SDL_LockSurface(screen) < 0) {
//				fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
//				return;
//			}
//		}
//
//		setPixel(i, gpu.curline, color_map[color]);
//
//		if (SDL_MUSTLOCK(screen)) {
//			SDL_UnlockSurface(screen);
//		}
//
//		x++;
//		if (x == 8) {
//			x = 0;
//			lineoffs = (lineoffs + 1) % 32;
////			tileoffset = map[mapoffs + lineoffs];
//			tileoffset = read_byte(0x8000 + mapoffs + lineoffs);
//			if (gpu.bg_tileset) { // Use signed tiles
////				curTile = gpu.tileset[0x100 + (s_byte) tileoffset];
////				curTile = (tile)(gpu.vram* + 0x100 + (s_byte) tileoffset);
//				memcpy(&curTile, gpu.vram + 0x100 + (s_byte) tileoffset, sizeof(curTile));
//			} else { // Use unsigned tiles
////				curTile = gpu.tileset[tileoffset];
////				curTile = (tile)(gpu.vram + tileoffset);
//				memcpy(&curTile, gpu.vram + tileoffset, sizeof(curTile));
//			}
//		}
//	}

}

void renderscreen_gpu() {
	// for(int i = 0; i < 144; i++) {
	// 	memcpy((void *)(SCREEN_BASE_ADDRESS + (i * SCREEN_WIDTH / 2)),
	// 			(const void *)screen, 80);
	// }

//	INFO("GPU: Updating the screen\n");

	SDL_UpdateRect(screen, 0, 0, 160, 144);
}

byte read_byte_gpu(word address) {
//	INFO("GPU: Reading word from 0x%04x...\n", address);
	switch (address) {
	case 0xFF40:
//		INFO("GPU: Read lcd_control: 0x%02x\n", gpu.lcd_control);
		return gpu.lcd_control;
	case 0xFF41:
		return gpu.lcdc_status;
	case 0xFF42:
//		INFO("GPU: Read scroll_y: 0x%02x\n", gpu.scroll_y);
		return gpu.scroll_y;
	case 0xFF43:
//		INFO("GPU: Read scroll_x: 0x%02x\n", gpu.scroll_x);
		return gpu.scroll_x;
	case 0xFF44:
//		INFO("GPU: Read curline: 0x%02x\n", gpu.curline);
		return gpu.curline;
	case 0xFF50:
		return mmu.in_bios;
	default:
		WARN("Tried to read from unimplemented GPU Register!! Returning 0...\n");
		return 0;
	}

	/* UNREACHABLE */
//	__builtin_unreachable();
}

void write_byte_gpu(word address, byte val) {
	switch (address) {
	case 0xFF40:
		gpu.lcd_control = val;
		break;
	case 0xFF41:
		gpu.lcdc_status = val;
		break;
	case 0xFF42:
		gpu.scroll_y = val;
		break;
	case 0xFF43:
		gpu.scroll_x = val;
		break;
	case 0xFF47:
		gpu.background_palette = val;
		break;
	case 0xFF50:
		mmu.in_bios = val;
		break;
	default:
		WARN("Tried to write 0x%x to unimplemented address 0x%x!  Skipping...\n", val, address);
	}

//	INFO("GPU: Wrote byte to 0x%04x, value 0x%02x\n", address, val);
}

void rendertile_gpu(word address) {
	INFO("Rendering tile...\n");
	word eff_addr = address & 0xFFF0;
	for (int i = 0; i < 8; i++) {
		for (int x = 0; x < 8; x ++) {
			byte low = gpu.vram[eff_addr + 2*i];
			byte high = gpu.vram[eff_addr + 2*i + 1];

			byte color_index = (byte)(((high >> x) & 0x1) << 1) | (byte)((low >> x) & 0x1);
			byte color = (gpu.background_palette >> (2 * color_index)) & 0x03;

			if (SDL_MUSTLOCK(screen)) {
				if (SDL_LockSurface(screen) < 0) {
					fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
					return;
				}
			}

			setPixel(162 + (address & 0x00F0)/2 + x, ((address & 0x0F00) >> 8) + i, color_map[color]);

			if (SDL_MUSTLOCK(screen)) {
				SDL_UnlockSurface(screen);
			}
		}
	}

	SDL_UpdateRect(screen, 161, 0, 289, 144);
}
