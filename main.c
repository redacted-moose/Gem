//#include <os.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>

#include "common.h"
#include "cpu.h"
#include "gpu.h"
#include "mmu.h"
#include "input.h"
#include "gem.yucc"

int main(int argc, char **argv) {

	// assert_ndless_rev(801);
	// enable_relative_paths(argv);
	// lcd_ingray();
	// clrscr();

	struct machine_t *gem = init();

	/* if (argc == 2) { */
	/* 	load_rom(gem->mmu, argv[1]); */
	/* 	INFO("Successfully loaded rom file %s\n", argv[1]); */
	/* } else { */
	/* 	load_rom(gem->mmu, "adjustris.gb.tns"); */
	/* 	INFO("Successfully loaded rom file %s\n", "adjustris.gb.tns"); */
	/* } */

	yuck_t argp[1];

	yuck_parse(argp, argc, argv);

	if (argp->debug_flag) {
		INFO("Debugging enabled\n");
	}

	if (argp->nargs > 0) {
		load_rom(gem->mmu, argp->args[0]);
		INFO("Successfully loaded rom file %s\n", argp->args[0]);
	}

	while (gem->run) {
        Uint32 t1 = SDL_GetTicks();

        while (gem->cpu->t < 70224) {
            execute(gem);
        }

        gem->cpu->t -= 70224;

        Uint32 delay = SDL_GetTicks() - t1;

        if (delay <= 17) {
            SDL_Delay(17 - delay);
        }
	}

    destroy(gem);

	yuck_free(argp);

	return 0;
}
