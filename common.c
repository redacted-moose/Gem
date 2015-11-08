/*
 * common.c
 */

#include <stdbool.h>

#include "common.h"
#include "cpu.h"
#include "gpu.h"
#include "mmu.h"
#include "input.h"

#ifdef DEBUG
struct machine_t *gem_debug;
#endif

struct machine_t *init() {
    struct machine_t *gem = malloc(sizeof(struct machine_t));
	gem->cpu = reset_cpu();
	gem->mmu = reset_mmu();
	gem->gpu = reset_gpu();
    gem->key = reset_key();
    gem->run = true;
#ifdef DEBUG
    gem_debug = gem;
#endif
    return gem;
}

void destroy(struct machine_t *gem) {
    destroy_cpu(gem->cpu);
    destroy_mmu(gem->mmu);
    destroy_gpu(gem->gpu);
    destroy_key(gem->key);
    free(gem);
}

void execute(struct machine_t *gem) {
    step_cpu(gem);
    INFO("Stepped CPU\n");
    do_dma(gem);
    INFO("DMA step\n");
    check_interrupts(gem);
    INFO("Interrupts checked\n");
    check_keys(gem);
    INFO("Keys checked\n");
    step_gpu(gem);
    INFO("GPU stepped\n");
}

