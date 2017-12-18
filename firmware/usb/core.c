#include <avr/io.h>

#define VENDOR_ID 0x046a
#define PRODUCT_ID 0x0001

#define read_bit(r, b) (r & (1 << b))
#define set_bit(r, b) (r |= (1 << b))
#define clear_bit(r, b) (r &= ~(1 << b))

static inline uint8_t recv8()
{
    return UEDATX;
}

static inline uint16_t recv16()
{
    uint16_t data;
    data = UEDATX;
    data |= UEDATX << 8;
    return data;
}

static inline void send8(uint8_t data)
{
    UEDATX = data;
}

static void init_endpoint(uint8_t endpoint, uint8_t cfg0, uint8_t cfg1)
{
    // Select endpoint
    UENUM = endpoint & 0b00000111;
    // Enable endpoint
    UECONX = 1;
    // Configure endpoint
    // See ATmega32U4 datasheet: 22.18.2 USB Device Endpoint Registers
    UECFG0X = cfg0;
    UECFG1X = cfg1;
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
    setup_packet->bmRequestType = recv8();
    setup_packet->bRequest = recv8();
    setup_packet->wValue = recv16();
    setup_packet->wIndex = recv16();
    setup_packet->wLength = recv16();
}

static void send_keyboard_report(usb_keyboard_report_t *usb_keyboard_report)
{
    send8(usb_keyboard_report->modifiers);
    send8(0);
    for (uint8_t i=0; i<6; i++)
    {
        send8(usb_keyboard_report->keys[i]);
    }
}
