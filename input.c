#include <stdbool.h>
#include <stdlib.h>

#ifdef SDL
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "common.h"
#include "input.h"
#include "mmu.h"

struct key_t* reset_key() {
    struct key_t* key = malloc(sizeof(struct key_t));
    key->rows[0] = 0xF;
    key->rows[0] = 0xF;
    key->column = 0;
    return key;
}

void destroy_key(struct key_t* key) { free(key); }

byte read_byte_key(struct machine_t* gem, word addr) {
    switch (gem->key->column) {
    case DIRECTIONAL_PAD:
        return gem->key->rows[0];
    case BUTTONS:
        return gem->key->rows[1];
    }
}

void write_byte_key(struct machine_t* gem, word addr, byte value) {
    gem->key->column = value & 0x30;
    /* if (value & 0x10) { INFO("Directional Pad selected\n"); } else {
     * INFO("Buttons selected\n"); } */
}

void check_keys(struct machine_t* gem) {
    SDL_Event event;
    struct mmu_t* mmu = gem->mmu;
    struct key_t* key = gem->key;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            gem->run = false;
            INFO("Exit requested\n");
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                key->rows[1] &= 0x0E;
                INFO("Right pressed\n");
                break;
            case SDLK_LEFT:
                key->rows[1] &= 0x0D;
                INFO("Left pressed\n");
                break;
            case SDLK_UP:
                key->rows[1] &= 0x0B;
                INFO("Up pressed\n");
                break;
            case SDLK_DOWN:
                key->rows[1] &= 0x07;
                INFO("Down pressed\n");
                break;
            case SDLK_z:
                key->rows[0] &= 0x0E;
                INFO("Z pressed\n");
                break;
            case SDLK_x:
                key->rows[0] &= 0x0D;
                INFO("X pressed\n");
                break;
            case SDLK_SPACE:
                key->rows[0] &= 0x0B;
                INFO("Space pressed\n");
                break;
            case SDLK_RETURN:
                key->rows[0] &= 0x07;
                INFO("Enter pressed\n");
                break;
            default:
                break;
            }
            mmu->i_flag.joypad = 1;
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                key->rows[1] |= 0x01;
                INFO("Right released\n");
                break;
            case SDLK_LEFT:
                key->rows[1] |= 0x02;
                INFO("Left released\n");
                break;
            case SDLK_UP:
                key->rows[1] |= 0x04;
                INFO("Up released\n");
                break;
            case SDLK_DOWN:
                key->rows[1] |= 0x08;
                INFO("Down released\n");
                break;
            case SDLK_z:
                key->rows[0] |= 0x01;
                INFO("Z released\n");
                break;
            case SDLK_x:
                key->rows[0] |= 0x02;
                INFO("X released\n");
                break;
            case SDLK_SPACE:
                key->rows[0] |= 0x04;
                INFO("Space released\n");
                break;
            case SDLK_RETURN:
                key->rows[0] |= 0x08;
                INFO("Enter released\n");
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}
