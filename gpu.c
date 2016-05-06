/*
 * gpu.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

//#include <os.h>

#include <stdio.h>
#include <stdlib.h>

#include "gpu.h"
#include "cpu.h"
#include "mmu.h"
#include "graphics.h"

void renderscan_gpu(struct machine_t*);
void renderscreen_gpu();

struct gpu_t* reset_gpu() {
    struct gpu_t* gpu = malloc(sizeof(struct gpu_t));
    gpu->mode = HBLANK;
    gpu->curline = 0;
    gpu->t = 0;
    reset_graphics();
    INFO("GPU Initialized\n");
    return gpu;
}

void destroy_gpu(struct gpu_t* gpu) {
    destroy_graphics();
    free(gpu);
}

/**
 * gpu state machine
 */
void step_gpu(struct machine_t* gem) {
    struct gpu_t* gpu = gem->gpu;
    struct cpu_t* cpu = gem->cpu;
    struct mmu_t* mmu = gem->mmu;

    switch (gpu->mode) {
    case OAM_READ:
        if (gpu->t >= OAM_READ_CYCLE_COUNT) {
            gpu->t = 0;
            gpu->mode = VRAM_READ;
            INFO("Entering VRAM Read mode\n");
        }
        break;

    case VRAM_READ:
        if (gpu->t >= VRAM_READ_CYCLE_COUNT) {
            gpu->t = 0;
            gpu->mode = HBLANK;
            INFO("Entering HBLANK\n");
            INFO("Rendering scanline...\n");
            renderscan_gpu(gem);
        }
        break;

    case HBLANK:
        if (gpu->t >= HBLANK_CYCLE_COUNT) {
            gpu->t = 0;
            gpu->curline++;
            if (gpu->curline == 144) {
                gpu->mode = VBLANK;
                INFO("Entering VBLANK\n");
                mmu->i_flag.vblank = 1;
                // plot to screen
                INFO("Refreshing screen\n");
                renderscreen_gpu();
            } else {
                gpu->mode = OAM_READ;
                INFO("Entering OAM Read mode\n");
            }
        }
        break;

    case VBLANK:
        if (gpu->t >= VBLANK_CYCLE_COUNT) {
            gpu->t = 0;
            gpu->curline++;

            if (gpu->curline > 153) {
                gpu->mode = OAM_READ;
                INFO("Entering OAM Read mode\n");
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
    byte color_index =
        (byte)(((r.a >> (7 - x)) & 0x1) << 1) | (byte)((r.b >> (7 - x)) & 0x1);
    return (p >> (2 * color_index)) & 0x03;
}

void render_background(struct machine_t* gem, byte scanrow[GPU_SCREEN_WIDTH]) {
    INFO("Rendering background scanline\n");
    struct gpu_t* gpu = gem->gpu;
    // which background tilemap will be used
    word mapoffs = gpu->bg_tilemap ? VRAM_BACKGROUND_TILE_MAP_1
                                   : VRAM_BACKGROUND_TILE_MAP_0;

    // which line to start at
    // this is a bit tricky - the division by 8 has to happen first
    mapoffs += 32 * ((gpu->curline + gpu->scroll_y) / 8);

    // which tile to start at in the map line
    byte lineoffs = gpu->scroll_x / 8;

    // which line in the tile to use
    byte y = (gpu->curline + gpu->scroll_y) % 8;

    // where to start in the tile line
    byte x = gpu->scroll_x % 8;

    for (int i = 0; i < 20; i++) { // for each tile in the current scanline
        byte tileoffset = read_byte(gem, mapoffs + lineoffs);

        tile cur_tile = gpu->tileset[(
            gpu->bg_tileset ? tileoffset
                            : (byte)((s_byte)0x100 + (s_byte)tileoffset))];

        row cur_row = cur_tile.rows[y];

        for (; x < 8; x++) {
            byte color = get_color(cur_row, x, gpu->background.palette);

            /* set_pixel(8*i + x, gpu.curline, color_map[color]); */
            scanrow[8 * i + x] = color;
        }

        x = 0;
        lineoffs = (lineoffs + 1) % 32;
    }
}

void render_sprites(struct machine_t* gem, byte scanrow[GPU_SCREEN_WIDTH]) {
    INFO("Rendering sprites\n");
    struct gpu_t* gpu = gem->gpu;
    for (int i = 0; i < 40; i++) { // for each sprite in sprite ram
        sprite obj = gpu->sprites[i];

        // check if this sprite falls on this scanline
        if (obj.y <= gpu->curline && (obj.y + 8) > gpu->curline) {
            INFO("Sprite %d is on the scanline!\n", i);
            // palette to use for this sprite
            palette pal = obj.palette ? gpu->object_1 : gpu->object_0;

            // data for this line of the sprite
            tile t = gpu->tileset[obj.tile_num];

            // if the sprite is y-flipped,
            // use the opposite side of the tile
            row tilerow = t.rows[obj.y_flip ? 7 - (gpu->curline - obj.y)
                                            : gpu->curline - obj.y];

            byte color;

            for (int x = 0; x < 8; x++) {
                // fixme: tilerow and scanrow
                // if this pixel is still on-screen, and
                // if it's not color 0 (transparent), and
                // if this sprite has priority or shows under the bg
                // then render the pixel
                if ((obj.x + x) >= 0 && (obj.x + x) < GPU_SCREEN_WIDTH &&
                    get_color(tilerow, x, pal.palette) &&
                    /* get_color(tilerow, x, 0b11100100) && */
                    /* (obj.priority || get_pixel(obj.x + x, gpu.curline) !=
                       0xff)) { */
                    !(obj.priority || !scanrow[obj.x + x])) {
                    // if the sprite is x-flipped,
                    // write pixels in reverse order
                    color = get_color(tilerow, obj.x_flip ? (7 - x) : x,
                                      pal.palette);
                    INFO("Pixel drawn at %d, %d, color: %d\n", obj.x + x, obj.y,
                         color);

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
void renderscan_gpu(struct machine_t* gem) {
    struct gpu_t* gpu = gem->gpu;

    byte scanrow[GPU_SCREEN_WIDTH] = {0};

    if (gpu->bg_on) {
        render_background(gem, scanrow);
    }

    if (gpu->spr_on) { // if sprites are enabled
        render_sprites(gem, scanrow);
    }

    for (int i = 0; i < GPU_SCREEN_WIDTH; i++) {
        set_pixel(i, gpu->curline, scanrow[i]);
    }
}

/**
 * blits the internal screen to SDL
 */
void renderscreen_gpu() {
    /* SDL_Flip(gem->gpu->screen); */
    blit_screen();
}

byte read_byte_gpu(struct gpu_t* gpu, word address) {
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
        WARN("Tried to read from unimplemented register 0x%04X!! Returning "
             "0...\n",
             address);
        return 0;
    }
}

void write_byte_gpu(struct gpu_t* gpu, word address, byte val) {
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
        WARN("Tried to write 0x%02X to unimplemented address 0x%04X!  "
             "skipping...\n",
             val, address);
    }
}
