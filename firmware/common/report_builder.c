#include "report_builder.h"

#include <stdbool.h>

static struct {
  // Keyboard report
  usb_report_keyboard_t *report_keyboard;
  // Number of usage codes in the keyboard report
  uint8_t keys_count;
  // Media keys report
  usb_report_media_t *report_media;
} report_builder;

void report_builder_init(usb_report_keyboard_t *report_keyboard,
                         usb_report_media_t *report_media) {
  report_builder.report_keyboard = report_keyboard;
  report_builder.report_media = report_media;
}

void report_builder_reset() {
  report_builder.keys_count = 0;

  // Empty the reports
  report_builder.report_keyboard->modifiers = 0;
  for (uint8_t i = 0; i < 6; i++) {
    report_builder.report_keyboard->keys[i] = 0;
  }
  report_builder.report_media->keys = 0;
}

// Return if there is no more space for usage codes
static bool is_full() { return (report_builder.keys_count >= 6); }

// Return if the code is already in the report
static bool has_key(uint8_t code) {
  for (uint8_t i = 0; i < report_builder.keys_count; i++) {
    if (report_builder.report_keyboard->keys[i] == code) return true;
  }
  return false;
}

void report_builder_add_key(uint8_t code) {
  if (is_full() || has_key(code)) return;
  report_builder.report_keyboard->keys[report_builder.keys_count++] = code;
}

void report_builder_add_modifier(uint8_t mask) {
  report_builder.report_keyboard->modifiers |= mask;
}

void report_builder_add_media(uint8_t mask) {
  report_builder.report_media->keys |= mask;
}
