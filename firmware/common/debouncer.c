#include "debouncer.h"

#include "config.h"
#include "controller.h"
#include "matrix.h"
#include <stdbool.h>
#include <stdint.h>

static struct {
    uint16_t history[MATRIX_KEYS];
    // Whether the debounced key is pressed or not
    bool pressed[MATRIX_KEYS];
} debouncer;

// Update the debouncer with the current state of each key in the matrix
void debouncer_update()
{
    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        // Left shift the key history: space is made on the right
        // for a new event, and the oldest event is shifted out
        debouncer.history[key] <<= 1;
        if (matrix_pressed[key]) debouncer.history[key] |= 1;

        if (debouncer.pressed[key])
        {
            // Key released for N cycles
            if ((debouncer.history[key] & DEBOUNCER_MASK) == 0)
            {
                debouncer.pressed[key] = false;
                controller_on_released(key);
            }
        }
        else
        {
            // Key pressed for N cycles
            if ((debouncer.history[key] & DEBOUNCER_MASK) == DEBOUNCER_MASK)
            {
                debouncer.pressed[key] = true;
                controller_on_pressed(key);
            }
        }
    }
}
