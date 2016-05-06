/*
 * gpu.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef GPU_H_
#define GPU_H_

#include <SDL/SDL.h>

#include "common.h"

// Memory locations
#define VRAM_TILE_DATA_TABLE_1      0x8000
#define VRAM_TILE_DATA_TABLE_0	    0x9000
#define VRAM_BACKGROUND_TILE_MAP_0  0x9800
#define VRAM_BACKGROUND_TILE_MAP_1  0x9C00

// Register aliases
#define GPU_LCD_CONTROL             0xFF40
#define GPU_LCDC_STATUS             0xFF41
#define GPU_SCROLL_Y                0xFF42
#define GPU_SCROLL_X                0xFF43
#define GPU_CURLINE                 0xFF44
#define GPU_CURLINE_COMPARE         0xFF45
#define GPU_BACKGROUND_PALETTE      0xFF47
#define GPU_OBJECT_PALETTE_0        0xFF48
#define GPU_OBJECT_PALETTE_1        0xFF49
#define GPU_WINDOW_Y                0xFF4A
#define GPU_WINDOW_X                0xFF4B // Actually window's x position - 7

#define GPU_SCREEN_WIDTH			160
#define GPU_SCREEN_HEIGHT			144

typedef enum gpu_mode {
	HBLANK = 0, VBLANK = 1, OAM_READ = 2, VRAM_READ = 3
} gpu_mode;

#define HBLANK_CYCLE_COUNT     204  // Between 201-207 clocks
#define VBLANK_CYCLE_COUNT     456  // 456 clocks per scanline - 10 scanlines of VBLANK - total 4560 cycles
#define OAM_READ_CYCLE_COUNT   80   // Between 77-83 clocks
#define VRAM_READ_CYCLE_COUNT  172  // Between 169-175 clocks
// Complete screen refresh should happen every 70224 clock cycles

typedef struct row_t {
    byte a;
    byte b;
} row;

typedef struct tile_t { // Make this a row or half row? sizeof(tile) = 16 bytes
	row rows[8];
} tile;

typedef struct sprite_t {

	struct {
		byte			:4;
		byte palette	:1;
		byte x_flip		:1;
		byte y_flip		:1;
		byte priority	:1;
	};

	byte tile_num;
	byte x;
	byte y;

} sprite;

typedef union palette_t {
	struct {
		byte color0 :2;
		byte color1 :2;
		byte color2 :2;
		byte color3 :2;
	};

	byte palette;
} palette;

struct gpu_t {
	union {
		byte vram[0x2000]; // 8 KB of VRAM
		struct {
			tile tileset[384];
			byte map0[1024];
            byte map1[1024];
        };
	};

	union {
		byte oam[0xA0]; // 160 bytes of OAM
		sprite sprites[40];
	};

	union {
		struct {
			gpu_mode mode : 2;
			byte : 6;
		};
		byte lcdc_status;
	};

	// GPU registers
	union {
		struct {
			// Bit 0
			byte bg_on			:1;
			byte spr_on			:1;
			byte spr_size		:1;
			byte bg_tilemap		:1;
			byte bg_tileset		:1;
			byte win_on			:1;
			byte win_tilemap	:1;
			byte display_on		:1;
      // Bit 7
		};

		byte lcd_control;
	};

	byte scroll_x;
	byte scroll_y;
	byte curline;
    byte curline_compare;
    byte window_x;
    byte window_y;

	palette background;

	palette object_0;
	palette object_1;

    int t; // Timing
};

struct gpu_t *reset_gpu();
void destroy_gpu(struct gpu_t *);
void step_gpu(struct machine_t *);
byte read_byte_gpu(struct gpu_t *, word);
void write_byte_gpu(struct gpu_t *, word, byte);

#endif /* GPU_H_ */
