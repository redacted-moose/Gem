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

GPUSTATE gpu;

SDL_Surface *screen;

void renderscan_gpu();
void renderscreen_gpu();

// FIXME: Palette is wrong color and is not grayscale
void reset_gpu() {
	gpu.mode = 0;
	gpu.curline = 0;
	SDL_Color colors[256];

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Make a grayscale color palette
	for(int i = 0; i < 256; i++){
		colors[i].r = i;
		colors[i].g = i;
		colors[i].b = i;
	}

	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, 8, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set %dx%dx8 video mode: %s\n",
				GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	// Set a grayscale color palette
	SDL_SetColors(screen, colors, 0, 256);

	SDL_WM_SetCaption("Gem - a Gameboy Emulator", NULL);
}

GPUSTATE get_gpu() {
	return gpu;
}

Uint32 color_map[4] = {
		0x000000FF, // White
		0x000000AA, // Light Gray
		0x00000055, // Dark Grey
		0x00000000  // Black
};

// This is sketchy...
void set_pixel(int x, int y, Uint32 color) {
//	unsigned char* p = (unsigned char*)(screen
//			+ ((x >> 1) + (y << 7) + (y << 5)));
//	*p = (x & 1) ? ((*p & 0xF0) | color) : ((*p & 0x0F) | (color << 4));
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }

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

    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }

}

/**
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 get_pixel(int x, int y)
{
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return 0;
        }
    }

    int bpp = screen->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }

    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }
}

/**
 * GPU State machine
 */
void step_gpu() {
	switch (gpu.mode) {
	case OAM_READ:
		if (cpu.t >= OAM_READ_CYCLE_COUNT) {
			cpu.t = 0;
			gpu.mode = VRAM_READ;
		}
		break;

	case VRAM_READ:
		if (cpu.t >= VRAM_READ_CYCLE_COUNT) {
			cpu.t = 0;
			gpu.mode = HBLANK;
			renderscan_gpu();
		}
		break;

	case HBLANK:
		if (cpu.t >= HBLANK_CYCLE_COUNT) {
			cpu.t = 0;
			gpu.curline++;
			if (gpu.curline == 144) {
				gpu.mode = VBLANK;
                mmu.i_flag.vblank = 1;
				// plot to screen
				renderscreen_gpu();
			} else {
				gpu.mode = OAM_READ;
			}
		}
		break;

	case VBLANK:
		if (cpu.t >= VBLANK_CYCLE_COUNT) {
			cpu.t = 0;
			gpu.curline++;

			if (gpu.curline > 153) {
				gpu.mode = OAM_READ;
				gpu.curline = 0;
			}
		}
		break;
	}
}

/**
 * Gets color of pixel x in row r using palette p
 */
byte get_color(row r, byte x, byte p) {
	byte color_index = (byte)(((r.a >> (7 - x)) & 0x1) << 1) | (byte)((r.b >> (7 - x)) & 0x1);
	return (p >> (2 * color_index)) & 0x03;
}

/**
 * Renders a scanline
 */
void renderscan_gpu() {
	byte scanrow[GPU_SCREEN_WIDTH];

	if (gpu.bg_on) {
		// Which background tilemap will be used
		word mapoffs = gpu.bg_tilemap ? VRAM_BACKGROUND_TILE_MAP_1 : VRAM_BACKGROUND_TILE_MAP_0;

		// Which line to start at
		// This is a bit tricky - the division by 8 has to happen first
		mapoffs += 32*((gpu.curline + gpu.scroll_y) / 8);

		// Which tile to start at in the map line
		byte lineoffs = gpu.scroll_x / 8;

		// Which line in the tile to use
		byte y = (gpu.curline + gpu.scroll_y) % 8;

		// Where to start in the tile line
		byte x = gpu.scroll_x % 8;

		for(int i = 0; i < 20; i++) {// For each tile in the current scanline
			byte tileoffset = read_byte(mapoffs + lineoffs);

			tile cur_tile = gpu.tileset[(gpu.bg_tileset ? tileoffset : 0x100 + (s_byte)tileoffset)];

			row cur_row = cur_tile.rows[y];

			for(; x < 8; x++) {
				byte color = get_color(cur_row, x, gpu.background.palette);

				/* set_pixel(8*i + x, gpu.curline, color_map[color]); */
                scanrow[8*i + x] = color;
			}

			x = 0;
			lineoffs = (lineoffs + 1) % 32;
		}
	}

	if (gpu.spr_on) {  // If sprites are enabled
		for(int i = 0; i < 40; i++) {  // for each sprite in sprite ram
			sprite obj = gpu.sprites[i];

			// Check if this sprite falls on this scanline
			if(obj.y <= gpu.curline && (obj.y + 8) > gpu.curline) {
				// Palette to use for this sprite
				palette pal = obj.palette ? gpu.object_1 : gpu.object_0;

				// Data for this line of the sprite
                tile t = gpu.tileset[obj.tile_num];

				// If the sprite is Y-flipped,
				// use the opposite side of the tile
                row tilerow = t.rows[obj.y_flip ? 7 - (gpu.curline - obj.y) : gpu.curline - obj.y];

				byte color;

				for(int x = 0; x < 8; x++) {
					// FIXME: tilerow and scanrow
					// If this pixel is still on-screen, AND
					// if it's not color 0 (transparent), AND
					// if this sprite has priority OR shows under the bg
					// then render the pixel
					if ((obj.x + x) >= 0 && (obj.x + x) < GPU_SCREEN_WIDTH &&
						get_color(tilerow, x, 0b11100100) &&
						/* (obj.priority || get_pixel(obj.x + x, gpu.curline) != 0xFF)) { */
                        (obj.priority || !scanrow[obj.x + x])) {
						// If the sprite is X-flipped,
						// write pixels in reverse order
						// color = pal[tilerow[obj.x_flip ? (7-x) : x]];
						color = get_color(tilerow, obj.x_flip ? (7-x) : x, pal.palette);

						/* set_pixel(obj.x + x, gpu.curline, color_map[color]); */
                        scanrow[obj.x + x] = color;
					}
                }
			}
		}
	}

    for (int i = 0; i < GPU_SCREEN_WIDTH; i++) {
        set_pixel(i, gpu.curline, color_map[scanrow[i]]);
    }
}

/**
 * Blits the internal screen to SDL
 */
void renderscreen_gpu() {
	/* SDL_UpdateRect(screen, 0, 0, GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT); */
    SDL_Flip(screen);
}

byte read_byte_gpu(word address) {
	switch (address) {
	case GPU_LCD_CONTROL:
		return gpu.lcd_control;
	case GPU_LCDC_STATUS:
		return gpu.lcdc_status;
	case GPU_SCROLL_Y:
		return gpu.scroll_y;
	case GPU_SCROLL_X:
		return gpu.scroll_x;
	case GPU_CURLINE:
		return gpu.curline;
	case GPU_BACKGROUND_PALETTE:
		return gpu.background.palette;
	case GPU_OBJECT_PALETTE_0:
		return gpu.object_0.palette;
	case GPU_OBJECT_PALETTE_1:
		return gpu.object_1.palette;
    case GPU_WINDOW_X:
        return gpu.window_x;
    case GPU_WINDOW_Y:
        return gpu.window_y;
	default:
		WARN("Tried to read from unimplemented GPU Register 0x%04X!! Returning 0...\n", address);
		return 0;
	}
}

void write_byte_gpu(word address, byte val) {
	switch (address) {
	case GPU_LCD_CONTROL:
		gpu.lcd_control = val;
		break;
	case GPU_LCDC_STATUS:
		gpu.lcdc_status = val;
		break;
	case GPU_SCROLL_Y:
		gpu.scroll_y = val;
		break;
	case GPU_SCROLL_X:
		gpu.scroll_x = val;
		break;
	case GPU_BACKGROUND_PALETTE:
		gpu.background.palette = val;
		break;
	case GPU_OBJECT_PALETTE_0:
		gpu.object_0.palette = val;
		break;
	case GPU_OBJECT_PALETTE_1:
		gpu.object_1.palette = val;
		break;
    case GPU_WINDOW_X:
        gpu.window_x = val;
        break;
    case GPU_WINDOW_Y:
        gpu.window_y = val;
	default:
		WARN("Tried to write 0x%02X to unimplemented address 0x%04X!  Skipping...\n", val, address);
	}
}
