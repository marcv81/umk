#include "controller.h"

#include "config.h"

#include "keys.h"
#include "usb.h"

#include <stdint.h>

#include <avr/pgmspace.h>

/*
 * Keyboard report builder
 */

static struct {
    uint8_t key_index;
    usb_keyboard_report_t report;
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
    keys_init();
    usb_init();
}

// Generated keymap
#include "keymap.c"

void controller_update()
{
    // Update everything
    keys_update();
    usb_update();

    keys_reset_layer();
    report_builder_reset();

    // Calculate the active layer and rebuild the keyboard report
    for (uint8_t key=0; key<MATRIX_KEYS; key++)
    {
        if (!keys_pressed(key)) continue;

        // Look up the key effect (type and value) from the appropriate layer
        uint8_t layer = keys_layer(key);
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
                keys_raise_layer(value);
                break;
        }
    }

    // Send the rebuilt keyboard report
    usb_keyboard_report_update(&report_builder.report);
}
