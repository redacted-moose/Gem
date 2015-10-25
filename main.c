//#include <os.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>

#include "cpu.h"
#include "gem.h"
#include "gpu.h"
#include "mmu.h"
#include "input.h"

bool run;

void reset();
void execute();

int main(int argc, char **argv) {

	// assert_ndless_rev(801);
	// enable_relative_paths(argv);
	// lcd_ingray();
	// clrscr();

	reset();

	if (argc == 2) {
		load_rom(argv[1]);
		INFO("Successfully loaded rom file %s\n", argv[1]);
	} else {
		load_rom("adjustris.gb.tns");
		INFO("Successfully loaded rom file %s\n", "adjustris.gb.tns");
	}

	while(run) {
        Uint32 t1 = SDL_GetTicks();

        while (cpu.t < 70224) {
            execute();
        }

        cpu.t -= 70224;

        Uint32 t2 = SDL_GetTicks();

        SDL_Delay(17 - (t2 - t1));
	}

    SDL_Quit();

    // Need to destroy here

	return 0;
}

void reset() {
    run = true;
	reset_cpu();
	reset_mmu();
	reset_gpu();
}

void execute() {
    step_cpu();
    do_dma();
    check_interrupts();
    check_keys();
    step_gpu();
}
