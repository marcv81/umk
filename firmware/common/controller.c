#include "controller.h"

#include "config.h"
#include "matrix.h"
#include "keys_list.h"
#include "layers.h"
#include "report_builder.h"
#include "usb.h"
#include <avr/pgmspace.h>
#include <stdbool.h>

void controller_on_pressed(uint8_t key)
{
    keys_list_add(key);
    layers_set(key);
}

void controller_on_released(uint8_t key)
{
    keys_list_remove(key);
}

#define KEY_TYPE_GENERAL 1
#define KEY_TYPE_MODIFIER 2
#define KEY_TYPE_LAYER 3

struct {
    usb_report_t report;
} controller;

void controller_init()
{
    matrix_init();
    usb_init();
}

// Generated keymap
#include "keymap.c"

void handle_key(uint8_t key)
{
    uint8_t layer = layers_get(key);

    // Look up the key effect (type and value) from the appropriate layer
    uint16_t offset = ((MATRIX_KEYS * layer) + key) << 1;
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
            layers_active_raise(value);
            break;
    }
}

void controller_update()
{
    // Update everything
    usb_update();
    matrix_update();

    // Calculate the active layer and rebuild the keyboard report
    layers_active_reset();
    report_builder_reset(&controller.report);
    keys_list_iterate(&handle_key);

    // Send the rebuilt keyboard report
    usb_report_send(&controller.report);
}
