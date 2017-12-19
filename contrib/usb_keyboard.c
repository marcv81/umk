/* USB Keyboard Example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2009 PJRC.COM, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Version 1.0: Initial Release
// Version 1.1: Add support for Teensy 2.0

#include "usb_keyboard.h"

#include "usb/core.c"
#include "usb/endpoints.c"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/**************************************************************************
 *
 *  Preprocessor macros
 *
 **************************************************************************/

#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

#if defined(__AVR_AT90USB162__)
#define HW_CONFIG()
#define PLL_CONFIG() (PLLCSR = ((1<<PLLE)|(1<<PLLP0)))
#define USB_CONFIG() (USBCON = (1<<USBE))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_ATmega32U4__)
#define HW_CONFIG() (UHWCON = 0x01)
#define PLL_CONFIG() (PLLCSR = 0x12)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB646__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x1A)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB1286__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x16)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#endif

// standard control endpoint request types
#define SET_ADDRESS                     5
#define GET_DESCRIPTOR                  6
#define SET_CONFIGURATION               9

/**************************************************************************
 *
 *  Endpoint Buffer Configuration
 *
 **************************************************************************/

#define KEYBOARD_INTERFACE      0
#define KEYBOARD_ENDPOINT       1

/**************************************************************************
 *
 *  Descriptor Data
 *
 **************************************************************************/

// Descriptors are the data that your computer reads when it auto-detects
// this USB device (called "enumeration" in USB lingo).  The most commonly
// changed items are editable at the top of this file.  Changing things
// in here should only be done by those who've read chapter 9 of the USB
// spec and relevant portions of any USB class specifications!

#define DESCRIPTOR_OFFSET_DEVICE 0
#define DESCRIPTOR_LENGTH_DEVICE 18

#define DESCRIPTOR_OFFSET_CONFIGURATION 18
#define DESCRIPTOR_LENGTH_CONFIGURATION 34

#define DESCRIPTOR_OFFSET_HID_INTERFACE 36
#define DESCRIPTOR_LENGTH_HID_INTERFACE 9

#define DESCRIPTOR_OFFSET_HID_REPORT 52
#define DESCRIPTOR_LENGTH_HID_REPORT 63

static const uint8_t PROGMEM descriptors[] = {

        // Device descriptor
        18,                                     // bLength
        1,                                      // bDescriptorType
        0x00, 0x02,                             // bcdUSB
        0,                                      // bDeviceClass
        0,                                      // bDeviceSubClass
        0,                                      // bDeviceProtocol
        ENDPOINT0_SIZE,                         // bMaxPacketSize0
        LSB(VENDOR_ID), MSB(VENDOR_ID),         // idVendor
        LSB(PRODUCT_ID), MSB(PRODUCT_ID),       // idProduct
        0x00, 0x01,                             // bcdDevice
        0,                                      // iManufacturer
        0,                                      // iProduct
        0,                                      // iSerialNumber
        1,                                      // bNumConfigurations

        // Configuration descriptor
        9,                                      // bLength;
        2,                                      // bDescriptorType;
        LSB(DESCRIPTOR_LENGTH_CONFIGURATION),   // wTotalLength
        MSB(DESCRIPTOR_LENGTH_CONFIGURATION),
        1,                                      // bNumInterfaces
        1,                                      // bConfigurationValue
        0,                                      // iConfiguration
        0xC0,                                   // bmAttributes
        50,                                     // bMaxPower

        // Interface descriptor
        9,                                      // bLength
        4,                                      // bDescriptorType
        KEYBOARD_INTERFACE,                     // bInterfaceNumber
        0,                                      // bAlternateSetting
        1,                                      // bNumEndpoints
        0x03,                                   // bInterfaceClass (0x03 = HID)
        0x01,                                   // bInterfaceSubClass (0x01 = Boot)
        0x01,                                   // bInterfaceProtocol (0x01 = Keyboard)
        0,                                      // iInterface

        // HID interface descriptor
        9,                                      // bLength
        0x21,                                   // bDescriptorType
        0x11, 0x01,                             // bcdHID
        0,                                      // bCountryCode
        1,                                      // bNumDescriptors
        0x22,                                   // bDescriptorType
        LSB(DESCRIPTOR_LENGTH_HID_REPORT),      // wDescriptorLength
        MSB(DESCRIPTOR_LENGTH_HID_REPORT),

        // Endpoint descriptor
        7,                                      // bLength
        5,                                      // bDescriptorType
        KEYBOARD_ENDPOINT | 0x80,               // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        LSB(ENDPOINT1_SIZE),                    // wMaxPacketSize
        MSB(ENDPOINT1_SIZE),
        1,                                      // bInterval

        // HID report descriptor
        0x05, 0x01,          // Usage Page (Generic Desktop),
        0x09, 0x06,          // Usage (Keyboard),
        0xA1, 0x01,          // Collection (Application),
        0x75, 0x01,          //   Report Size (1),
        0x95, 0x08,          //   Report Count (8),
        0x05, 0x07,          //   Usage Page (Key Codes),
        0x19, 0xE0,          //   Usage Minimum (224),
        0x29, 0xE7,          //   Usage Maximum (231),
        0x15, 0x00,          //   Logical Minimum (0),
        0x25, 0x01,          //   Logical Maximum (1),
        0x81, 0x02,          //   Input (Data, Variable, Absolute), ;Modifier byte
        0x95, 0x01,          //   Report Count (1),
        0x75, 0x08,          //   Report Size (8),
        0x81, 0x03,          //   Input (Constant),                 ;Reserved byte
        0x95, 0x05,          //   Report Count (5),
        0x75, 0x01,          //   Report Size (1),
        0x05, 0x08,          //   Usage Page (LEDs),
        0x19, 0x01,          //   Usage Minimum (1),
        0x29, 0x05,          //   Usage Maximum (5),
        0x91, 0x02,          //   Output (Data, Variable, Absolute), ;LED report
        0x95, 0x01,          //   Report Count (1),
        0x75, 0x03,          //   Report Size (3),
        0x91, 0x03,          //   Output (Constant),                 ;LED report padding
        0x95, 0x06,          //   Report Count (6),
        0x75, 0x08,          //   Report Size (8),
        0x15, 0x00,          //   Logical Minimum (0),
        0x25, 0x68,          //   Logical Maximum(104),
        0x05, 0x07,          //   Usage Page (Key Codes),
        0x19, 0x00,          //   Usage Minimum (0),
        0x29, 0x68,          //   Usage Maximum (104),
        0x81, 0x00,          //   Input (Data, Array),
        0xc0,                // End Collection
};

/**************************************************************************
 *
 *  Variables - these are the only non-stack RAM usage
 *
 **************************************************************************/

usb_keyboard_report_t usb_keyboard_report;

static bool update_requested = false;

/**************************************************************************
 *
 *  Public Functions - these are the API intended for the user
 *
 **************************************************************************/

// initialize USB
void usb_init(void)
{
        HW_CONFIG();
        USB_FREEZE();   // enable USB
        PLL_CONFIG();                           // config PLL
        while (!(PLLCSR & (1<<PLOCK))) ;        // wait for PLL lock
        USB_CONFIG();                           // start USB clock
        UDCON = 0;                              // enable attach resistor
}

// send the contents of keyboard_keys and keyboard_modifier_keys
void usb_keyboard_send()
{
        update_requested = true;
}

#include "usb/descriptors.c"

#include "usb/more.c"
