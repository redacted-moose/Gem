#include <SDL2/SDL.h>

#include "../graphics.h"
#include "../gpu.h"

enum {
    BLACK = 0x00000000,
    DARK_GRAY = 0x00555555,
    LIGHT_GRAY = 0x00AAAAAA,
    WHITE = 0x00FFFFFF
};

const Uint32 colorMap[4] = {WHITE, LIGHT_GRAY, DARK_GRAY, BLACK};

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* screen;
Uint32 pixels[GPU_SCREEN_WIDTH * GPU_SCREEN_HEIGHT];

void reset_graphics() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Couldn't initialize sdl2: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_Quit);

    window =
        SDL_CreateWindow("Gem - a Gameboy emulator", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, GPU_SCREEN_WIDTH,
                         GPU_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Couldn't get a %dx%d window: %s\n", GPU_SCREEN_WIDTH,
                GPU_SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        fprintf(stderr, "Couldn't get %dx%d renderer: %s\n", GPU_SCREEN_WIDTH,
                GPU_SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING, GPU_SCREEN_WIDTH,
                               GPU_SCREEN_HEIGHT);
    if (screen == NULL) {
        fprintf(stderr, "Couldn't get %dx%d texture: %s\n", GPU_SCREEN_WIDTH,
                GPU_SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }
}

void destroy_graphics() {
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void set_pixel(int x, int y, byte color) {
    pixels[GPU_SCREEN_WIDTH * y + x] = colorMap[color];
    /* int offset = GPU_SCREEN_WIDTH*y + x; */
    /* switch (color) { */
    /* case 0: */
    /*   pixels[offset] = WHITE; */
    /* case 1: */
    /*   pixels[offset] = LIGHT_GRAY; */
    /* case 2: */
    /*   pixels[offset] = DARK_GRAY; */
    /* case 3: */
    /*   pixels[offset] = BLACK; */
    /* } */
}

byte get_pixel(int x, int y) { return 0; }

void blit_screen() {
    SDL_UpdateTexture(screen, NULL, &pixels, GPU_SCREEN_WIDTH * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);
}
