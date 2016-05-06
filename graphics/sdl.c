#include <SDL/SDL.h>

#include "../graphics.h"

#define COLOR_WHITE 0x000000ff
#define COLOR_LIGHT_GRAY 0x000000aa
#define COLOR_DARK_GRAY 0x00000055
#define COLOR_BLACK 0x00000000

const Uint32 color_map[4] = {COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY,
                             COLOR_BLACK};

SDL_Surface* screen;

void reset_graphics() {
    SDL_Color colors[256];

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Couldn't initialize sdl: %s\n", SDL_GetError());
        exit(1);
    }

    // Make a grayscale color palette
    for (int i = 0; i < 256; i++) {
        colors[i].r = i;
        colors[i].g = i;
        colors[i].b = i;
    }

    atexit(SDL_Quit);

    screen =
        SDL_SetVideoMode(GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, 8, SDL_SWSURFACE);
    if (screen == NULL) {
        fprintf(stderr, "Couldn't set %dx%dx8 video mode: %s\n",
                GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    // Set a grayscale color palette
    SDL_SetColors(screen, colors, 0, 256);

    SDL_WM_SetCaption("Gem - a gameboy emulator", NULL);
}

void destroy_graphics() { SDL_Quit(); }

void set_pixel(int x, int y, byte byte_color) {
    //     unsigned char* p = (unsigned char*)(screen
    //                     + ((x >> 1) + (y << 7) + (y << 5)));
    //     *p = (x & 1) ? ((*p & 0xf0) | color) : ((*p & 0x0f) | (color << 4));

    Uint32 color = color_map[byte_color];

    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }

    int bpp = screen->format->BytesPerPixel;
    /* here p is the address to the pixel we want to set */
    Uint8* p = (Uint8*)screen->pixels + y * screen->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = color;
        break;

    case 2:
        *(Uint16*)p = color;
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
        *(Uint32*)p = color;
        break;
    }

    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }
}

Uint32 get_pixel_raw(int x, int y) {
    int bpp = screen->format->BytesPerPixel;
    /* here p is the address to the pixel we want to retrieve */
    Uint8* p = (Uint8*)screen->pixels + y * screen->pitch + x * bpp;

    switch (bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16*)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32*)p;

    default:
        return 0; /* shouldn't happen, but avoids warnings */
    }
}

/**
 * return the pixel value at (x, y)
 * note: the surface must be locked before calling this!
 */
// FIXME: This is broken - needs to return a byte and properly unlock screen
byte get_pixel(int x, int y) {
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return 0;
        }
    }

    Uint32 color = get_pixel_raw(x, y);

    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }

    switch (color) {
    case COLOR_WHITE:
        return 0;

    case COLOR_LIGHT_GRAY:
        return 1;

    case COLOR_DARK_GRAY:
        return 2;

    case COLOR_BLACK:
        return 3;

    default:
        return 0;
    }
}

void blit_screen() { SDL_Flip(screen); }
