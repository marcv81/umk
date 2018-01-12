#ifndef USB_H
#define USB_H

#include <stdint.h>

void usb_init();
void usb_update();

typedef struct {
    uint8_t modifiers;
    uint8_t keys[6];
} usb_keyboard_report_t;

void usb_keyboard_report_update(usb_keyboard_report_t *report);

#endif // USB_H
