#ifndef INPUT_H_
#define INPUT_H_

#include "common.h"

typedef enum key_mode_t {
	BUTTONS = 2, DIRECTIONAL_PAD = 1
} key_mode;

struct key_t {
	byte rows[2];
	byte column;
};

struct key_t *reset_key();
void destroy_key(struct key_t *);
byte read_byte_key(struct machine_t *, word);
void write_byte_key(struct machine_t *, word, byte);
void check_keys(struct machine_t *);

#endif
