#include "keys.h"

#include "config.h"

#include "matrix.h"

/*
 * Layers tracking
 */

static struct {
    // Active layer
    uint8_t active;
    // Active layer at the time the key was last pressed
    uint8_t pressed[MATRIX_KEYS];
} layers;

void keys_reset_layer()
{
    layers.active = 0;
}

void keys_raise_layer(uint8_t layer)
{
    if (layer > layers.active) layers.active = layer;
}

uint8_t keys_layer(uint8_t key)
{
    return layers.pressed[key];
}

// Called when a debounced key transitions from released to pressed
static inline void layers_on_pressed(uint8_t key) {
    layers.pressed[key] = layers.active;
}

/*
 * Keys debouncer
 */

static struct {
    // Last 16 matrix states for the key
    uint16_t history[MATRIX_KEYS];
    // Whether the debounced key is pressed or not
    bool pressed[MATRIX_KEYS];
} debouncer;

static inline void debouncer_update()
{
    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        // Insert the latest state and shift out the oldest one
        debouncer.history[key] <<= 1;
        if (matrix_pressed(key)) debouncer.history[key] |= 1;

        if (!debouncer.pressed[key])
        {
            // Key pressed for N cycles
            if ((debouncer.history[key] & DEBOUNCER_MASK) == DEBOUNCER_MASK)
            {
                debouncer.pressed[key] = true;
                layers_on_pressed(key);
            }
        }
        else
        {
            // Key released for N cycles
            if ((debouncer.history[key] & DEBOUNCER_MASK) == 0)
            {
                debouncer.pressed[key] = false;
            }
        }
    }
}

bool keys_pressed(uint8_t key)
{
    return debouncer.pressed[key];
}

/*
 * Keys core
 */

void keys_init()
{
    matrix_init();
}

void keys_update()
{
    matrix_update();
    debouncer_update();
}
