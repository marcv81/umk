#include "keymap.h"

#include <avr/pgmspace.h>

#include "config.h"
#include "keymap.inc"

keycode_t keymap_load(uint8_t key, uint8_t layer) {
  return (keycode_t){
      pgm_read_byte(&keymap[layer][key].type),
      pgm_read_byte(&keymap[layer][key].value),
  };
}
