#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdint.h>

typedef enum { None, Key, Modifier, Layer } keycode_type_t;

typedef struct {
    keycode_type_t type;
    uint8_t value;
} keycode_t;

keycode_t keymap_load(uint8_t key, uint8_t layer);

#endif // KEYMAP_H
