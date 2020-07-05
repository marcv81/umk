#ifndef LAYERS_H
#define LAYERS_H

#include <stdint.h>

void layers_active_reset();
void layers_active_raise(uint8_t layer);

void layers_set(uint8_t key);
uint8_t layers_get(uint8_t key);

#endif  // LAYERS_H
