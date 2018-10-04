#include "controller.h"

#include "config.h"
#include "matrix.h"
#include "keymap.h"
#include "keys_list.h"
#include "layers.h"
#include "report_builder.h"
#include "usb.h"

void controller_on_pressed(uint8_t key)
{
    keys_list_add(key);
    layers_set(key);
}

void controller_on_released(uint8_t key)
{
    keys_list_remove(key);
}

struct {
    usb_report_t report;
} controller;

void controller_init()
{
    matrix_init();
    usb_init();
}

void handle_key(uint8_t key)
{
    keycode_t keycode;
    keymap_load(key, layers_get(key), &keycode);

    switch (keycode.type)
    {
        case Normal:
        {
            report_builder_add_key(keycode.value);
        }
        break;
        case Modifier:
        {
            report_builder_add_modifier(keycode.value);
        }
        break;
        case Layer:
        {
            layers_active_raise(keycode.value);
        }
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
