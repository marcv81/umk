#include "core.h"

#include "config.h"
#include "debouncer.h"
#include "keymap.h"
#include "keys_list.h"
#include "layers.h"
#include "matrix.h"
#include "report_builder.h"
#include "usb.h"

static struct {
    usb_report_t report;
} core;

static void on_pressed(uint8_t key)
{
    keys_list_add(key);
    layers_set(key);
}

static void on_released(uint8_t key)
{
    keys_list_remove(key);
}

static void rebuild(uint8_t key)
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

void core_init()
{
    usb_init();
    matrix_init(&debouncer_recv);
    debouncer_init(&on_pressed, &on_released);
    report_builder_init(&core.report);
}

void core_update()
{
    usb_update();
    matrix_scan();

    // Rebuild the report and active layer
    report_builder_reset();
    layers_active_reset();
    keys_list_iterate(&rebuild);

    usb_report_send(&core.report);
}
