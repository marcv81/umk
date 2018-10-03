#ifndef REPORT_BUILDER_H
#define REPORT_BUILDER_H

#include "usb.h"

void report_builder_reset(usb_report_t *report);
void report_builder_add_key(uint8_t code);
void report_builder_add_modifier(uint8_t mask);

#endif // REPORT_BUILDER_H
