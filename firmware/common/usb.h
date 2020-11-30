#ifndef USB_H
#define USB_H

#include <stdint.h>

void usb_init();
void usb_update();

typedef struct {
  uint8_t modifiers;
  uint8_t keys[6];
} usb_report_keyboard_t;

typedef struct {
  uint8_t keys;
} usb_report_media_t;

void usb_send_report_keyboard(const usb_report_keyboard_t *report);
void usb_send_report_media(const usb_report_media_t *report);

#endif  // USB_H
