#ifndef INPUT_H_
#define INPUT_H_

#include "gem.h"

typedef enum key_mode_t {
	BUTTONS = 2, DIRECTIONAL_PAD = 1
} key_mode;

typedef struct key_t {
	byte rows[2];
	byte column;
} KEYSTATE;

extern KEYSTATE key;

void reset_key();
byte read_byte_key(word);
void write_byte_key(word, byte);
void check_keys();

#endif
