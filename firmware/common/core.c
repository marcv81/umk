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
  usb_report_keyboard_t report_keyboard;
  usb_report_media_t report_media;
  bool flip;
} core;

static void on_pressed(uint8_t key) {
  keys_list_add(key);
  layers_set(key);
}

static void on_released(uint8_t key) { keys_list_remove(key); }

static void rebuild(uint8_t key) {
  keycode_t keycode = keymap_load(key, layers_get(key));

  switch (keycode.type) {
    case Key: {
      report_builder_add_key(keycode.value);
    } break;
    case Modifier: {
      report_builder_add_modifier(keycode.value);
    } break;
    case Media: {
      report_builder_add_media(keycode.value);
    } break;
    case Layer: {
      layers_active_raise(keycode.value);
    } break;
    case Timer: {
      if (core.flip) {
        core.flip = false;
      }
      else {
        core.flip = true;
        report_builder_add_modifier(0b00000010);
      }
    }
  }
}

void core_init() {
  usb_init();
  matrix_init(&debouncer_recv);
  debouncer_init(&on_pressed, &on_released);
  report_builder_init(&core.report_keyboard, &core.report_media);
}

void core_update() {
  usb_update();
  matrix_scan();

  // Rebuild the report and active layer
  report_builder_reset();
  layers_active_reset();
  keys_list_iterate(&rebuild);

  usb_send_report_keyboard(&core.report_keyboard);
  usb_send_report_media(&core.report_media);
}
