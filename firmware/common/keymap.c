#include "keymap.h"

#include "config.h"
#include <avr/pgmspace.h>

#include "keymap.inc"

void keymap_load(uint8_t key, uint8_t layer, keycode_t *keycode)
{
    keycode->type = pgm_read_byte(&keymap[layer][key].type);
    keycode->value = pgm_read_byte(&keymap[layer][key].value);
}
