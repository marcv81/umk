#ifndef REPORT_BUILDER_H
#define REPORT_BUILDER_H

#include "usb.h"

void report_builder_init(usb_report_keyboard_t *report_keyboard,
                         usb_report_media_t *report_media);

void report_builder_reset();
void report_builder_add_key(uint8_t code);
void report_builder_add_modifier(uint8_t mask);
void report_builder_add_media(uint8_t mask);

#endif  // REPORT_BUILDER_H
