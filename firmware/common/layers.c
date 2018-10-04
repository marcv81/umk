#include "layers.h"

#include "config.h"

static struct {
    // Active layer
    uint8_t active;
    // Active layer when a key was last set
    uint8_t keys[MATRIX_KEYS];
} layers;

void layers_active_reset()
{
    layers.active = 0;
}

void layers_active_raise(uint8_t layer)
{
    if (layer > layers.active) layers.active = layer;
}

void layers_set(uint8_t key)
{
    layers.keys[key] = layers.active;
}

uint8_t layers_get(uint8_t key)
{
    return layers.keys[key];
}
