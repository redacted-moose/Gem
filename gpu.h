/*
 * gpu.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef GPU_H_
#define GPU_H_

#include "gem.h"

typedef enum gpu_mode {
	HBLANK = 0, VBLANK = 1, OAM_READ = 2, VRAM_READ = 3
} gpu_mode;

typedef struct row_t {
	union {
		byte a;
		struct {
			byte a0 :1;
			byte a1 :1;
			byte a2 :1;
			byte a3 :1;
			byte a4 :1;
			byte a5 :1;
			byte a6 :1;
			byte a7 :1;
		};
	};

	union {
		byte b;
		struct {
			byte b0 :1;
			byte b1 :1;
			byte b2 :1;
			byte b3 :1;
			byte b4 :1;
			byte b5 :1;
			byte b6 :1;
			byte b7 :1;
		};
	};
} row;

typedef struct tile_t { // Make this a row or half row? sizeof(tile) = 16 bytes
	row rows[8];
} tile;

typedef struct sprite_t {
	struct {
		byte :4;
		byte palette :1;
		byte x_flip :1;
		byte y_flip :1;
		byte priority :1;
	};
	byte tile_num;
	byte x;
	byte y;
} sprite;

typedef struct gpu_t {
	byte vram[0x2000]; // 8 KB of VRAM - restructure as array of tiles + 2 1KB tilemaps
			// union of vram byte array and tileset/maps?
//	union {
//		byte vram[0x2000]; // 8 KB of VRAM - restructure as array of tiles + 2 1KB tilemaps
//		// union of vram byte array and tileset/maps?
//		struct {
//			// High address
//			tile tileset[384];
//			byte map1[1024];
//			byte map0[1024];
//		// Low address
//		};
//	};
	union {
		byte oam[0xA0]; // 160 bytes of OAM
		sprite sprites[40];
	};

//	gpu_mode mode;

	union {
		struct {
			byte mode : 2;
			byte : 6;
		};

		byte lcdc_status;
	};

	// GPU registers

	union {
		struct {
			// Bit 0
			byte bg_on :1;
			byte spr_on :1;
			byte spr_size :1;
			byte bg_tilemap :1;
			byte bg_tileset :1;
			byte win_on :1;
			byte win_tilemap :1;
			byte display_on :1;
		// Bit 7
		};

		byte lcd_control;
	};
	byte scroll_x;
	byte scroll_y;
	byte curline;
	union {
		struct {
			byte color0 :2;
			byte color1 :2;
			byte color2 :2;
			byte color3 :2;
		};
		byte background_palette;
	};
} GPU;

extern GPU gpu;

void reset_gpu();
void step_gpu();
byte read_byte_gpu(word);
void write_byte_gpu(word, byte);

#endif /* GPU_H_ */
