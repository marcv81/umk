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

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/**************************************************************************
 *
 *  Preprocessor macros
 *
 **************************************************************************/

#define EP_TYPE_CONTROL                 0x00
#define EP_TYPE_INTERRUPT_IN            0xC1

#define EP_SINGLE_BUFFER                0x02
#define EP_DOUBLE_BUFFER                0x06

#define EP_SIZE(s)      ((s) == 64 ? 0x30 :     \
                        ((s) == 32 ? 0x20 :     \
                        ((s) == 16 ? 0x10 :     \
                                     0x00)))

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
#define GET_STATUS                      0
#define SET_ADDRESS                     5
#define GET_DESCRIPTOR                  6
#define GET_CONFIGURATION               8
#define SET_CONFIGURATION               9
// HID (human interface device)
#define HID_GET_REPORT                  1
#define HID_GET_IDLE                    2
#define HID_GET_PROTOCOL                3
#define HID_SET_REPORT                  9
#define HID_SET_IDLE                    10
#define HID_SET_PROTOCOL                11

/**************************************************************************
 *
 *  Endpoint Buffer Configuration
 *
 **************************************************************************/

#define ENDPOINT0_SIZE          32

#define KEYBOARD_INTERFACE      0
#define KEYBOARD_ENDPOINT       1
#define KEYBOARD_SIZE           8
#define KEYBOARD_BUFFER         EP_DOUBLE_BUFFER

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
        KEYBOARD_SIZE, 0,                       // wMaxPacketSize
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

// zero when we are not configured, non-zero when enumerated
static volatile uint8_t usb_configuration=0;

usb_keyboard_report_t usb_keyboard_report;

// protocol setting from the host.  We use exactly the same report
// either way, so this variable only stores the setting since we
// are required to be able to report which setting is in use.
static uint8_t keyboard_protocol=1;

// the idle configuration, how often we send the report to the
// host (ms * 4) even when it hasn't changed
static uint8_t keyboard_idle_config=125;

// count until idle timeout
static uint8_t keyboard_idle_count=0;

// 1=num lock, 2=caps lock, 4=scroll lock, 8=compose, 16=kana
volatile uint8_t keyboard_leds=0;

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
        usb_configuration = 0;
        UDIEN = (1<<EORSTE)|(1<<SOFE);
        sei();
}

// return 0 if the USB is not configured, or the configuration
// number selected by the HOST
uint8_t usb_configured(void)
{
        return usb_configuration;
}

// send the contents of keyboard_keys and keyboard_modifier_keys
int8_t usb_keyboard_send(void)
{
        uint8_t i, intr_state, timeout;

        if (!usb_configuration) return -1;
        intr_state = SREG;
        cli();
        UENUM = KEYBOARD_ENDPOINT;
        timeout = UDFNUML + 50;
        while (1) {
                // are we ready to transmit?
                if (UEINTX & (1<<RWAL)) break;
                SREG = intr_state;
                // has the USB gone offline?
                if (!usb_configuration) return -1;
                // have we waited too long?
                if (UDFNUML == timeout) return -1;
                // get ready to try checking again
                intr_state = SREG;
                cli();
                UENUM = KEYBOARD_ENDPOINT;
        }
        send_keyboard_report(&usb_keyboard_report);
        UEINTX = 0x3A;
        keyboard_idle_count = 0;
        SREG = intr_state;
        return 0;
}

/**************************************************************************
 *
 *  Private Functions - not intended for general user consumption....
 *
 **************************************************************************/

// USB Device Interrupt - handle all device-level events
// the transmit buffer flushing is triggered by the start of frame
//
ISR(USB_GEN_vect)
{
        uint8_t intbits, t, i;
        static uint8_t div4=0;

        intbits = UDINT;
        UDINT = 0;
        if (intbits & (1<<EORSTI)) {
                init_endpoint(0, EP_TYPE_CONTROL, EP_SIZE(ENDPOINT0_SIZE) | EP_SINGLE_BUFFER);
                UEIENX = (1<<RXSTPE);
                usb_configuration = 0;
        }
        if ((intbits & (1<<SOFI)) && usb_configuration) {
                if (keyboard_idle_config && (++div4 & 3) == 0) {
                        UENUM = KEYBOARD_ENDPOINT;
                        if (UEINTX & (1<<RWAL)) {
                                keyboard_idle_count++;
                                if (keyboard_idle_count == keyboard_idle_config) {
                                        keyboard_idle_count = 0;
                                        send_keyboard_report(&usb_keyboard_report);
                                        UEINTX = 0x3A;
                                }
                        }
                }
        }
}

// Misc functions to wait for ready and send/receive packets
static inline void usb_wait_in_ready(void)
{
        while (!(UEINTX & (1<<TXINI))) ;
}
static inline void usb_send_in(void)
{
        UEINTX = ~(1<<TXINI);
}
static inline void usb_wait_receive_out(void)
{
        while (!(UEINTX & (1<<RXOUTI))) ;
}
static inline void usb_ack_out(void)
{
        UEINTX = ~(1<<RXOUTI);
}

#include "usb/descriptors.c"

// USB Endpoint Interrupt - endpoint 0 is handled here.  The
// other endpoints are manipulated by the user-callable
// functions, and the start-of-frame interrupt.
//
ISR(USB_COM_vect)
{
        setup_packet_t setup_packet;

        UENUM = 0;
        if (UEINTX & (1<<RXSTPI)) {
                recv_setup_packet(&setup_packet);
                clear_bit(UEINTX, RXSTPI);
                if (setup_packet.bRequest == GET_DESCRIPTOR) {
                        descriptor_send_dispatch(&setup_packet);
                        return;
                }
                if (setup_packet.bRequest == SET_ADDRESS) {
                        usb_send_in();
                        usb_wait_in_ready();
                        UDADDR = setup_packet.wValue | (1<<ADDEN);
                        return;
                }
                if (setup_packet.bRequest == SET_CONFIGURATION && setup_packet.bmRequestType == 0) {
                        usb_configuration = setup_packet.wValue;
                        usb_send_in();
                        init_endpoint(1, EP_TYPE_INTERRUPT_IN, EP_SIZE(KEYBOARD_SIZE) | KEYBOARD_BUFFER);
                        UERST = 0b00000010;
                        UERST = 0;
                        return;
                }
                if (setup_packet.bRequest == GET_CONFIGURATION && setup_packet.bmRequestType == 0x80) {
                        usb_wait_in_ready();
                        UEDATX = usb_configuration;
                        usb_send_in();
                        return;
                }

                if (setup_packet.bRequest == GET_STATUS) {
                        usb_wait_in_ready();
                        UEDATX = 0;
                        UEDATX = 0;
                        usb_send_in();
                        return;
                }
                if (setup_packet.wIndex == KEYBOARD_INTERFACE) {
                        if (setup_packet.bmRequestType == 0xA1) {
                                if (setup_packet.bRequest == HID_GET_REPORT) {
                                        usb_wait_in_ready();
                                        send_keyboard_report(&usb_keyboard_report);
                                        usb_send_in();
                                        return;
                                }
                                if (setup_packet.bRequest == HID_GET_IDLE) {
                                        usb_wait_in_ready();
                                        UEDATX = keyboard_idle_config;
                                        usb_send_in();
                                        return;
                                }
                                if (setup_packet.bRequest == HID_GET_PROTOCOL) {
                                        usb_wait_in_ready();
                                        UEDATX = keyboard_protocol;
                                        usb_send_in();
                                        return;
                                }
                        }
                        if (setup_packet.bmRequestType == 0x21) {
                                if (setup_packet.bRequest == HID_SET_REPORT) {
                                        usb_wait_receive_out();
                                        keyboard_leds = UEDATX;
                                        usb_ack_out();
                                        usb_send_in();
                                        return;
                                }
                                if (setup_packet.bRequest == HID_SET_IDLE) {
                                        keyboard_idle_config = (setup_packet.wValue >> 8);
                                        keyboard_idle_count = 0;
                                        usb_send_in();
                                        return;
                                }
                                if (setup_packet.bRequest == HID_SET_PROTOCOL) {
                                        keyboard_protocol = setup_packet.wValue;
                                        usb_send_in();
                                        return;
                                }
                        }
                }
        }
        UECONX = (1<<STALLRQ) | (1<<EPEN);      // stall
}
