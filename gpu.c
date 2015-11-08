/*
 * gpu.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

//#include <os.h>

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "gpu.h"
#include "cpu.h"
#include "mmu.h"

void renderscan_gpu(struct machine_t *);
void renderscreen_gpu(struct machine_t *);

SDL_Surface *init_gfx() {
    SDL_Surface *screen;
    SDL_Color colors[256];

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "couldn't initialize sdl: %s\n", SDL_GetError());
		exit(1);
	}

	// make a grayscale color palette
	for(int i = 0; i < 256; i++){
		colors[i].r = i;
		colors[i].g = i;
		colors[i].b = i;
	}

	atexit(SDL_Quit);

	screen = SDL_SetVideoMode(GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, 8, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf(stderr, "couldn't set %dx%dx8 video mode: %s\n",
				GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	// set a grayscale color palette
	SDL_SetColors(screen, colors, 0, 256);

	SDL_WM_SetCaption("gem - a gameboy emulator", NULL);

    return screen;
}

struct gpu_t *reset_gpu() {
    struct gpu_t *gpu = malloc(sizeof(struct gpu_t));
	gpu->mode = HBLANK;
	gpu->curline = 0;
    gpu->screen = init_gfx();
    return gpu;
}

void destroy_gpu(struct gpu_t *gpu) {
    SDL_Quit();
    free(gpu);
}

Uint32 color_map[4] = {
		0x000000ff, // white
		0x000000aa, // light gray
		0x00000055, // dark grey
		0x00000000  // black
};

// this is sketchy...
void set_pixel(struct gpu_t *gpu, int x, int y, Uint32 color) {
//	unsigned char* p = (unsigned char*)(screen
//			+ ((x >> 1) + (y << 7) + (y << 5)));
//	*p = (x & 1) ? ((*p & 0xf0) | color) : ((*p & 0x0f) | (color << 4));
    if (SDL_MUSTLOCK(gpu->screen)) {
        if (SDL_LockSurface(gpu->screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }

	int bpp = gpu->screen->format->BytesPerPixel;
	/* here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *) gpu->screen->pixels + y * gpu->screen->pitch + x * bpp;

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

    if (SDL_MUSTLOCK(gpu->screen)) {
        SDL_UnlockSurface(gpu->screen);
    }

}

/**
 * return the pixel value at (x, y)
 * note: the surface must be locked before calling this!
 */
Uint32 get_pixel(struct gpu_t *gpu, int x, int y)
{
    if (SDL_MUSTLOCK(gpu->screen)) {
        if (SDL_LockSurface(gpu->screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return 0;
        }
    }

    int bpp = gpu->screen->format->BytesPerPixel;
    /* here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)gpu->screen->pixels + y * gpu->screen->pitch + x * bpp;

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

    if (SDL_MUSTLOCK(gpu->screen)) {
        SDL_UnlockSurface(gpu->screen);
    }
}

/**
 * gpu state machine
 */
void step_gpu(struct machine_t *gem) {
    struct gpu_t *gpu = gem->gpu;
    struct cpu_t *cpu = gem->cpu;
    struct mmu_t *mmu = gem->mmu;

	switch (gpu->mode) {
	case OAM_READ:
		if (cpu->t >= OAM_READ_CYCLE_COUNT) {
			cpu->t = 0;
			gpu->mode = VRAM_READ;
            INFO("[GPU] Entering VRAM Read mode\n");
		}
		break;

	case VRAM_READ:
		if (cpu->t >= VRAM_READ_CYCLE_COUNT) {
			cpu->t = 0;
			gpu->mode = HBLANK;
            INFO("[GPU] Entering HBLANK\n");
            INFO("[GPU] Rendering scanline...\n");
			renderscan_gpu(gem);
		}
		break;

	case HBLANK:
		if (cpu->t >= HBLANK_CYCLE_COUNT) {
			cpu->t = 0;
			gpu->curline++;
			if (gpu->curline == 144) {
				gpu->mode = VBLANK;
                INFO("[GPU] Entering VBLANK\n");
                mmu->i_flag.vblank = 1;
				// plot to screen
                INFO("[GPU] Refreshing screen\n");
				renderscreen_gpu(gem);
			} else {
				gpu->mode = OAM_READ;
                INFO("[GPU] Entering OAM Read mode\n");
			}
		}
		break;

	case VBLANK:
		if (cpu->t >= VBLANK_CYCLE_COUNT) {
			cpu->t = 0;
			gpu->curline++;

			if (gpu->curline > 153) {
				gpu->mode = OAM_READ;
                INFO("[GPU] Entering OAM Read mode\n");
				gpu->curline = 0;
			}
		}
		break;
	}
}

/**
 * gets color of pixel x in row r using palette p
 */
byte get_color(row r, byte x, byte p) {
	byte color_index = (byte)(((r.a >> (7 - x)) & 0x1) << 1) | (byte)((r.b >> (7 - x)) & 0x1);
	return (p >> (2 * color_index)) & 0x03;
}

void render_background(struct machine_t *gem, byte scanrow[GPU_SCREEN_WIDTH]) {
    struct gpu_t *gpu = gem->gpu;
    // which background tilemap will be used
    word mapoffs = gpu->bg_tilemap ? VRAM_BACKGROUND_TILE_MAP_1 : VRAM_BACKGROUND_TILE_MAP_0;

    // which line to start at
    // this is a bit tricky - the division by 8 has to happen first
    mapoffs += 32*((gpu->curline + gpu->scroll_y) / 8);

    // which tile to start at in the map line
    byte lineoffs = gpu->scroll_x / 8;

    // which line in the tile to use
    byte y = (gpu->curline + gpu->scroll_y) % 8;

    // where to start in the tile line
    byte x = gpu->scroll_x % 8;

    for(int i = 0; i < 20; i++) {// for each tile in the current scanline
        byte tileoffset = read_byte(gem, mapoffs + lineoffs);

        tile cur_tile = gpu->tileset[(gpu->bg_tileset ? tileoffset : 0x100 + (s_byte)tileoffset)];

        row cur_row = cur_tile.rows[y];

        for(; x < 8; x++) {
            byte color = get_color(cur_row, x, gpu->background.palette);

            /* set_pixel(8*i + x, gpu.curline, color_map[color]); */
            scanrow[8*i + x] = color;
        }

        x = 0;
        lineoffs = (lineoffs + 1) % 32;
    }
}

void render_sprites(struct machine_t *gem, byte scanrow[GPU_SCREEN_WIDTH]) {
    struct gpu_t *gpu = gem->gpu;
    for(int i = 0; i < 40; i++) {  // for each sprite in sprite ram
        sprite obj = gpu->sprites[i];

        // check if this sprite falls on this scanline
        if(obj.y <= gpu->curline && (obj.y + 8) > gpu->curline) {
            // palette to use for this sprite
            palette pal = obj.palette ? gpu->object_1 : gpu->object_0;

            // data for this line of the sprite
            tile t = gpu->tileset[obj.tile_num];

            // if the sprite is y-flipped,
            // use the opposite side of the tile
            row tilerow = t.rows[obj.y_flip ? 7 - (gpu->curline - obj.y) : gpu->curline - obj.y];

            byte color;

            for(int x = 0; x < 8; x++) {
                // fixme: tilerow and scanrow
                // if this pixel is still on-screen, and
                // if it's not color 0 (transparent), and
                // if this sprite has priority or shows under the bg
                // then render the pixel
                if ((obj.x + x) >= 0 && (obj.x + x) < GPU_SCREEN_WIDTH &&
                    get_color(tilerow, x, 0b11100100) &&
                    /* (obj.priority || get_pixel(obj.x + x, gpu.curline) != 0xff)) { */
                    (obj.priority || !scanrow[obj.x + x])) {
                    // if the sprite is x-flipped,
                    // write pixels in reverse order
                    color = get_color(tilerow, obj.x_flip ? (7-x) : x, pal.palette);

                    /* set_pixel(obj.x + x, gpu.curline, color_map[color]); */
                    scanrow[obj.x + x] = color;
                }
            }
        }
    }
}

/**
 * renders a scanline
 */
void renderscan_gpu(struct machine_t *gem) {
    struct gpu_t *gpu = gem->gpu;

	byte scanrow[GPU_SCREEN_WIDTH] = {0};

	if (gpu->bg_on) {
        render_background(gem, scanrow);
    }

	if (gpu->spr_on) {  // if sprites are enabled
        render_sprites(gem, scanrow);
	}

    for (int i = 0; i < GPU_SCREEN_WIDTH; i++) {
        set_pixel(gpu, i, gpu->curline, color_map[scanrow[i]]);
    }
}

/**
 * blits the internal screen to sdl
 */
void renderscreen_gpu(struct machine_t *gem) {
	/* sdl_updaterect(screen, 0, 0, gpu_screen_width, gpu_screen_height); */
    SDL_Flip(gem->gpu->screen);
}

byte read_byte_gpu(struct gpu_t *gpu, word address) {
	switch (address) {
	case GPU_LCD_CONTROL:
		return gpu->lcd_control;
	case GPU_LCDC_STATUS:
		return gpu->lcdc_status;
	case GPU_SCROLL_Y:
		return gpu->scroll_y;
	case GPU_SCROLL_X:
		return gpu->scroll_x;
	case GPU_CURLINE:
		return gpu->curline;
	case GPU_BACKGROUND_PALETTE:
		return gpu->background.palette;
	case GPU_OBJECT_PALETTE_0:
		return gpu->object_0.palette;
	case GPU_OBJECT_PALETTE_1:
		return gpu->object_1.palette;
    case GPU_WINDOW_X:
        return gpu->window_x;
    case GPU_WINDOW_Y:
        return gpu->window_y;
	default:
		WARN("[GPU] Tried to read from unimplemented register 0x%04X!! Returning 0...\n", address);
		return 0;
	}
}

void write_byte_gpu(struct gpu_t *gpu, word address, byte val) {
	switch (address) {
	case GPU_LCD_CONTROL:
		gpu->lcd_control = val;
		break;
	case GPU_LCDC_STATUS:
		gpu->lcdc_status = val;
		break;
	case GPU_SCROLL_Y:
		gpu->scroll_y = val;
		break;
	case GPU_SCROLL_X:
		gpu->scroll_x = val;
		break;
	case GPU_BACKGROUND_PALETTE:
		gpu->background.palette = val;
		break;
	case GPU_OBJECT_PALETTE_0:
		gpu->object_0.palette = val;
		break;
	case GPU_OBJECT_PALETTE_1:
		gpu->object_1.palette = val;
		break;
    case GPU_WINDOW_X:
        gpu->window_x = val;
        break;
    case GPU_WINDOW_Y:
        gpu->window_y = val;
	default:
		WARN("[GPU] Tried to write 0x%02X to unimplemented address 0x%04X!  skipping...\n", val, address);
	}
}
