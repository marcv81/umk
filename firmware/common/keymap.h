#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdint.h>

typedef enum { Normal, Modifier, Layer } keycode_type_t;

typedef struct {
    keycode_type_t type;
    uint8_t value;
} keycode_t;

void keymap_load(uint8_t key, uint8_t layer, keycode_t *keycode);

#endif // KEYMAP_H
