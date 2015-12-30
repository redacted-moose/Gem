#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "gem.h"

#define GPU_SCREEN_WIDTH			160
#define GPU_SCREEN_HEIGHT			144

void reset_graphics();
void destroy_graphics();
void blit_screen();
void set_pixel(int, int, byte);
byte get_pixel(int, int);

#endif /* GRAPHICS_H_ */
