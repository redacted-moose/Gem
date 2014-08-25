/*
 * gpu.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include <os.h>
#include <string.h>
#include "gem.h"
#include "gpu.h"
#include "cpu.h"

GPU gpu;

void renderscan_gpu();
void renderscreen_gpu();

void reset_gpu()
{
	gpu.mode = 0;
	gpu.curline = 0;
}

byte screen[160 * 144 / 2];

int color_map[4] = {0xF, // WHITE
		0xA, 0x5, 0x0 // BLACK
		};

// This is sketchy...
void setPixel(int x, int y, int color)
{
	unsigned char* p = (unsigned char*)(screen
			+ ((x >> 1) + (y << 7) + (y << 5)));
	*p = (x & 1) ? ((*p & 0xF0) | color) : ((*p & 0x0F) | (color << 4));
}

void step_gpu()
{
	switch(gpu.mode) {
	case OAM_READ:
		if(cpu.t >= 80) {
			cpu.t = 0;
			gpu.mode = VRAM_READ;
		}
		break;

	case VRAM_READ:
		if(cpu.t >= 172) {
			cpu.t = 0;
			gpu.mode = HBLANK;

			renderscan_gpu();
		}
		break;

	case HBLANK:
		if(cpu.t >= 204) {
			cpu.t = 0;
			gpu.curline++;
			if(gpu.curline == 143) {
				gpu.mode = VBLANK;
				// plot to screen
				renderscreen_gpu();
			} else {
				gpu.mode = OAM_READ;
			}
		}
		break;

	case VBLANK:
		if(cpu.t >= 456) {
			cpu.t = 0;
			gpu.curline++;

			if(gpu.curline > 153) {
				gpu.mode = OAM_READ;
				gpu.curline = 0;
			}
		}
		break;
	}
}

void renderscan_gpu()
{
	byte *map = (gpu.bg_tilemap) ? gpu.map1 : gpu.map0;
	byte mapoffs = (gpu.curline + gpu.scroll_y) / 8;
	byte lineoffs = gpu.scroll_x / 8;
	byte y = (gpu.curline + gpu.scroll_y) % 8;
	byte x = gpu.scroll_x % 8;
//	byte screenoffs = gpu.curline * 160;
	byte tileoffset = map[mapoffs + lineoffs];
	tile curTile;

	if(gpu.bg_tileset) { // Use signed tiles
		curTile = gpu.tileset[0x100 + (s_byte)tileoffset];
	} else { // Use unsigned tiles
		curTile = gpu.tileset[tileoffset];
	}

	for(int i = 0; i < 160; i++) {
		byte color;
		row curRow = curTile.rows[y];
		byte color_index;
		switch(x) {
		case 0:
			color_index = curRow.a0 | (curRow.b0 << 1);
			break;
		case 1:
			color_index = curRow.a1 | (curRow.b1 << 1);
			break;
		case 2:
			color_index = curRow.a2 | (curRow.b2 << 1);
			break;
		case 3:
			color_index = curRow.a3 | (curRow.b3 << 1);
			break;
		case 4:
			color_index = curRow.a4 | (curRow.b4 << 1);
			break;
		case 5:
			color_index = curRow.a5 | (curRow.b5 << 1);
			break;
		case 6:
			color_index = curRow.a6 | (curRow.b6 << 1);
			break;
		case 7:
			color_index = curRow.a7 | (curRow.b7 << 1);
			break;
		}
		color = (gpu.background_palette >> (2 * color_index)) & 0x03;

		setPixel(i, gpu.curline, color_map[color]);

		x++;
		if(x == 8) {
			x = 0;
			lineoffs = (lineoffs + 1) % 32;
			tileoffset = map[mapoffs + lineoffs];
			if(gpu.bg_tileset) { // Use signed tiles
				curTile =
						gpu.tileset[0x100
								+ (s_byte)tileoffset];
			} else { // Use unsigned tiles
				curTile = gpu.tileset[tileoffset];
			}
		}
	}

}

void renderscreen_gpu()
{
	for(int i = 0; i < 144; i++) {
		memcpy((void *)(SCREEN_BASE_ADDRESS + (i * SCREEN_WIDTH / 2)),
				(const void *)screen, 80);
	}
}

byte read_byte_gpu(word address)
{
	switch(address) {
	case 0xFF40:
		return gpu.lcd_control;
	case 0xFF42:
		return gpu.scroll_y;
	case 0xFF43:
		return gpu.scroll_x;
	case 0xFF44:
		return gpu.curline;
	}
}

void write_byte_gpu(word address, byte val)
{
	switch(address) {
	case 0xFF40:
		gpu.lcd_control = val;
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
	}
}
