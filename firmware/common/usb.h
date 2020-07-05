#ifndef USB_H
#define USB_H

#include <stdint.h>

void usb_init();
void usb_update();

typedef struct {
  uint8_t modifiers;
  uint8_t keys[6];
} usb_report_t;

void usb_report_send(const usb_report_t *report);

#endif  // USB_H
