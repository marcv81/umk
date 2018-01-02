#ifndef USB_H
#define USB_H

#include <stdint.h>

void usb_init();
void usb_update();

typedef struct
{
    uint8_t modifiers;
    uint8_t keys[6];
}
usb_keyboard_report_t;

// Keyboard report
extern usb_keyboard_report_t usb_keyboard_report;

// Send the keyboard report to the host at the next opportunity
void usb_keyboard_report_update();

#endif // USB_H
