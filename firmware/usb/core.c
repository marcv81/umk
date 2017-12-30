#include <avr/io.h>

#define VENDOR_ID 0x046a
#define PRODUCT_ID 0x0001

#define read_bit(r, b) (r & (1 << b))
#define set_bit(r, b) (r |= (1 << b))
#define clear_bit(r, b) (r &= ~(1 << b))

void usb_init()
{
    // Enable the pad regulator
    set_bit(UHWCON, UVREGE);
    // Enable the USB controller
    set_bit(USBCON, USBE);
    // Configure the PLL divisor (16MHz clock)
    set_bit(PLLCSR, PINDIV);
    // Enable the PLL
    set_bit(PLLCSR, PLLE);
    // Wait for the PLL to lock
    while (!read_bit(PLLCSR, PLOCK));
    // Enable the VBUS pad
    set_bit(USBCON, OTGPADE);
    // Enable the clock
    clear_bit(USBCON, FRZCLK);
    // Attach the device
    clear_bit(UDCON, DETACH);
}

typedef struct
{
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
}
setup_packet_t;

static void recv_setup_packet(setup_packet_t *setup_packet)
{
    setup_packet->bmRequestType = UEDATX;
    setup_packet->bRequest = UEDATX;
    setup_packet->wValue = UEDATX;
    setup_packet->wValue |= UEDATX << 8;
    setup_packet->wIndex = UEDATX;
    setup_packet->wIndex |= UEDATX << 8;
    setup_packet->wLength = UEDATX;
    setup_packet->wLength |= UEDATX << 8;
}

static void send_keyboard_report(usb_keyboard_report_t *usb_keyboard_report)
{
    UEDATX = usb_keyboard_report->modifiers;
    UEDATX = 0;
    for (uint8_t i=0; i<6; i++)
    {
        UEDATX = usb_keyboard_report->keys[i];
    }
}
