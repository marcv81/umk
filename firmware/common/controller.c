#include "controller.h"

#include "config.h"
#include "matrix.h"
#include "usb.h"
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * Matrix debouncer
 */

static struct {
    uint16_t history[MATRIX_KEYS];
} debouncer;

// Update the debouncer with the current state of each key in the matrix
static void debouncer_update()
{
    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        // Left shift the key history: space is made on the right
        // for a new event, and the oldest event is shifted out
        debouncer.history[key] <<= 1;
        if (matrix_pressed[key]) debouncer.history[key] |= 1;
    }
}

static inline bool debouncer_released(uint8_t key)
{
    // Key released for N cycles
    return (debouncer.history[key] & DEBOUNCER_MASK) == 0;
}

static inline bool debouncer_pressed(uint8_t key)
{
    // Key pressed for N cycles
    return (debouncer.history[key] & DEBOUNCER_MASK) == DEBOUNCER_MASK;
}

/*
 * Layers manager
 */

static struct {
    // Active layer
    uint8_t active;
    // Active layer at the time a key was pressed, or 0 if released
    uint8_t pressed[MATRIX_KEYS];
} layers_manager;

// Reset the active layer
static void layers_manager_reset_active()
{
    layers_manager.active = 1;
}

// Raise the active layer
static void layers_manager_raise_active(uint8_t layer)
{
    if (layer > layers_manager.active) layers_manager.active = layer;
}

// Update the pressed layer for newly pressed/released keys
static void layers_manager_update_pressed()
{
    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        if (layers_manager.pressed[key] == 0)
        {
            if (debouncer_pressed(key))
            {
                layers_manager.pressed[key] = layers_manager.active;
            }
        }
        else
        {
            if (debouncer_released(key))
            {
                layers_manager.pressed[key] = 0;
            }
        }
    }
}

/*
 * Keyboard report builder
 */

static struct {
    uint8_t key_index;
    usb_report_t report;
} report_builder;

static void report_builder_reset()
{
    report_builder.key_index = 0;
    for (uint8_t i=0; i<6; i++)
    {
        report_builder.report.keys[i] = 0;
    }
    report_builder.report.modifiers = 0;
}

static void report_builder_add_key(uint8_t code)
{
    if (report_builder.key_index >= 6) return;
    for (uint8_t i=0; i<report_builder.key_index; i++)
    {
        if (report_builder.report.keys[i] == code) return;
    }
    report_builder.report.keys[report_builder.key_index++] = code;
}

static void report_builder_add_modifier(uint8_t mask)
{
    report_builder.report.modifiers |= mask;
}

/*
 * Controller core
 */

#define KEY_TYPE_GENERAL 1
#define KEY_TYPE_MODIFIER 2
#define KEY_TYPE_LAYER 3

void controller_init()
{
    matrix_init();
    usb_init();
}

// Generated keymap
#include "keymap.c"

void controller_update()
{
    // Update everything
    usb_update();
    matrix_update();
    debouncer_update();
    layers_manager_update_pressed();

    layers_manager_reset_active();
    report_builder_reset();

    // Calculate the active layer and rebuild the keyboard report
    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        uint8_t layer = layers_manager.pressed[key];
        if (layer == 0) continue;

        // Look up the key effect (type and value) from the appropriate layer
        uint16_t offset = ((MATRIX_KEYS * (layer - 1)) + key) << 1;
        uint8_t type = pgm_read_byte(keymap + offset);
        uint8_t value = pgm_read_byte(keymap + offset + 1);

        switch (type)
        {
            // General keys
            case KEY_TYPE_GENERAL:
                report_builder_add_key(value);
                break;

            // Modifier keys
            case KEY_TYPE_MODIFIER:
                report_builder_add_modifier(value);
                break;

            // Layer keys
            case KEY_TYPE_LAYER:
                layers_manager_raise_active(value);
                break;
        }
    }

    // Send the rebuilt keyboard report
    usb_report_send(&report_builder.report);
}
