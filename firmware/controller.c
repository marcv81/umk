#include "controller.h"

#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>

#include "config.h"
#include "matrix.h"
#include "usb.h"

/*
 * Key debouncer
 */

uint8_t debouncer_history[MATRIX_KEYS];

// Update the debouncer with the current state of each key in the matrix
static void update_debouncer()
{
    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        // Left shift the key history: space is made on the right
        // for a new event, and the oldest event is shifted out
        debouncer_history[key] <<= 1;
        if (matrix_pressed[key]) debouncer_history[key] |= 1;
    }
}

static inline bool is_released(uint8_t key)
{
    // Key released for N cycles
    return (debouncer_history[key] & DEBOUNCER_MASK) == 0;
}

static inline bool is_pressed(uint8_t key)
{
    // Key pressed for N cycles
    return (debouncer_history[key] & DEBOUNCER_MASK) == DEBOUNCER_MASK;
}

/*
 * Pressed keys/layers
 */

static uint8_t active_layer = 1;

// Pressed keys/layers: active layer at the time
// a key was pressed, or 0 if not currently pressed
static uint8_t pressed_layer[MATRIX_KEYS];

// Update the pressed keys/layers from the current
// (debounced) state of each key and the active layer
static void update_pressed_layer()
{
    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        if (pressed_layer[key] == 0)
        {
            if (is_pressed(key)) pressed_layer[key] = active_layer;
        }
        else
        {
            if (is_released(key)) pressed_layer[key] = 0;
        }
    }
}

/*
 * Keymap effects lookup
 */

#define KEY_TYPE_GENERAL 1
#define KEY_TYPE_MODIFIER 2
#define KEY_TYPE_LAYER 3

// Generated keymap
#include "keymap.c"

usb_keyboard_report_t report;

// Update the active layer and the keyboard report
// from the pressed keys/layers and the keymap
static void update_keymap()
{
    // Reset the active layer
    active_layer = 1;

    // Reset the keyboard report
    report.modifiers = 0;
    for (uint8_t i=0; i<6; i++) report.keys[i] = 0;

    uint8_t key_index = 0;

    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        uint8_t layer = pressed_layer[key];
        if (layer == 0) continue;

        // Look up the key effect (type and value) from the appropriate layer
        uint16_t offset = ((MATRIX_KEYS * (layer - 1)) + key) << 1;
        uint8_t type = pgm_read_byte(keymap + offset);
        uint8_t value = pgm_read_byte(keymap + offset + 1);

        switch (type)
        {
            // General keys
            case KEY_TYPE_GENERAL:
                if (key_index < 6) report.keys[key_index++] = value;
                break;

            // Modifier keys
            case KEY_TYPE_MODIFIER:
                report.modifiers |= value;
                break;

            // Layer keys
            case KEY_TYPE_LAYER:
                if (value > active_layer) active_layer = value;
                break;
        }
    }
}

/*
 * Coroutine
 */

void controller_update()
{
    update_debouncer();
    update_pressed_layer();
    update_keymap();
    usb_keyboard_report_update(&report);
}
