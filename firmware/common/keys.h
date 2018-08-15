#ifndef KEYS_H
#define KEYS_H

#include <stdbool.h>
#include <stdint.h>

void keys_init();
void keys_update();

bool keys_pressed(uint8_t key);

void keys_reset_layer();
void keys_raise_layer(uint8_t layer);
uint8_t keys_layer(uint8_t key);

#endif // KEYS_H
