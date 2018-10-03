#include "usb.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

#define read_bit(r, b) (r & (1 << b))
#define set_bit(r, b) (r |= (1 << b))
#define clear_bit(r, b) (r &= ~(1 << b))

#define lsb(n) (n & 255)
#define msb(n) ((n >> 8) & 255)
#define min(x, y) ((x) < (y) ? (x) : (y))

/*
 * USB initialization
 */

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

/*
 * Endpoints configuration
 */

// Endpoint configuration register 0 constants
#define ENDPOINT_CFG0_TYPE_CONTROL   0b00000000
#define ENDPOINT_CFG0_TYPE_INTERRUPT 0b11000000
#define ENDPOINT_CFG0_DIRECTION_OUT  0b00000000
#define ENDPOINT_CFG0_DIRECTION_IN   0b00000001

// Endpoint configuration register 1 constants
#define ENDPOINT_CFG1_SIZE_8      0b00000000
#define ENDPOINT_CFG1_SIZE_16     0b00010000
#define ENDPOINT_CFG1_SIZE_32     0b00100000
#define ENDPOINT_CFG1_SIZE_64     0b00110000
#define ENDPOINT_CFG1_SIZE_128    0b01000000
#define ENDPOINT_CFG1_SIZE_256    0b01010000
#define ENDPOINT_CFG1_SIZE_512    0b01100000
#define ENDPOINT_CFG1_BANK_SINGLE 0b00000000
#define ENDPOINT_CFG1_BANK_DOUBLE 0b00000100
#define ENDPOINT_CFG1_ALLOCATE    0b00000010

// Control endpoint configuration
#define ENDPOINT0_SIZE 64
#define ENDPOINT0_CFG0 ( \
    ENDPOINT_CFG0_TYPE_CONTROL | \
    ENDPOINT_CFG0_DIRECTION_OUT)
#define ENDPOINT0_CFG1 ( \
    ENDPOINT_CFG1_SIZE_64 | \
    ENDPOINT_CFG1_BANK_SINGLE | \
    ENDPOINT_CFG1_ALLOCATE)

// Keyboard endpoint configuration
#define ENDPOINT1_SIZE 8
#define ENDPOINT1_CFG0 ( \
    ENDPOINT_CFG0_TYPE_INTERRUPT | \
    ENDPOINT_CFG0_DIRECTION_IN)
#define ENDPOINT1_CFG1 ( \
    ENDPOINT_CFG1_SIZE_8 | \
    ENDPOINT_CFG1_BANK_DOUBLE | \
    ENDPOINT_CFG1_ALLOCATE)

#define MASK_UENUM 0b00000111

// Select endpoint
#define endpoint_select(e) (UENUM = (e & MASK_UENUM))

// Configure endpoint
static void endpoint_init(uint8_t endpoint, uint8_t cfg0, uint8_t cfg1)
{
    endpoint_select(endpoint);
    // Enable endpoint
    set_bit(UECONX, EPEN);
    // Set endpoint configuration registers
    UECFG0X = cfg0;
    UECFG1X = cfg1;
    // Wait until endpoint is configured
    while (!read_bit(UESTA0X, CFGOK));
}

// Configure the endpoints after a device reset
static void update_reset()
{
    // Return unless an end-of-reset event happened
    if (!read_bit(UDINT, EORSTI)) return;

    // Acknowledge the event
    clear_bit(UDINT, EORSTI);

    // Configure the endpoints
    endpoint_init(0, ENDPOINT0_CFG0, ENDPOINT0_CFG1);
    endpoint_init(1, ENDPOINT1_CFG0, ENDPOINT1_CFG1);
}

/*
 * USB descriptors
 */

#define VENDOR_ID 0x046a // Cherry
#define PRODUCT_ID 0x0001 // Keyboard

// Standard descriptor types
#define DESCRIPTOR_TYPE_DEVICE 0x01
#define DESCRIPTOR_TYPE_CONFIGURATION 0x02
#define DESCRIPTOR_TYPE_INTERFACE 0x04
#define DESCRIPTOR_TYPE_ENDPOINT 0x05
#define DESCRIPTOR_TYPE_HID_INTERFACE 0x21
#define DESCRIPTOR_TYPE_HID_REPORT 0x22

// Standard descriptor lengths
#define DESCRIPTOR_LENGTH_DEVICE 18
#define DESCRIPTOR_LENGTH_CONFIGURATION 9
#define DESCRIPTOR_LENGTH_INTERFACE 9
#define DESCRIPTOR_LENGTH_ENDPOINT 7
#define DESCRIPTOR_LENGTH_HID_INTERFACE 9

// Descriptors offsets
#define DESCRIPTOR_OFFSET_DEVICE 0
#define DESCRIPTOR_OFFSET_CONFIGURATION ( \
    DESCRIPTOR_OFFSET_DEVICE + \
    DESCRIPTOR_LENGTH_DEVICE)
#define DESCRIPTOR_OFFSET_INTERFACE ( \
    DESCRIPTOR_OFFSET_CONFIGURATION + \
    DESCRIPTOR_LENGTH_CONFIGURATION)
#define DESCRIPTOR_OFFSET_HID_INTERFACE ( \
    DESCRIPTOR_OFFSET_INTERFACE + \
    DESCRIPTOR_LENGTH_INTERFACE)
#define DESCRIPTOR_OFFSET_ENDPOINT ( \
    DESCRIPTOR_OFFSET_HID_INTERFACE + \
    DESCRIPTOR_LENGTH_HID_INTERFACE)
#define DESCRIPTOR_OFFSET_HID_REPORT ( \
    DESCRIPTOR_OFFSET_ENDPOINT + \
    DESCRIPTOR_LENGTH_ENDPOINT)

// Total configuration length
#define DESCRIPTOR_LENGTH_CONFIGURATION_TOTAL ( \
    DESCRIPTOR_LENGTH_CONFIGURATION + \
    DESCRIPTOR_LENGTH_INTERFACE + \
    DESCRIPTOR_LENGTH_HID_INTERFACE + \
    DESCRIPTOR_LENGTH_ENDPOINT)

// HID report length
# define DESCRIPTOR_LENGTH_HID_REPORT 63

static const uint8_t PROGMEM descriptors[] = {

    // Device descriptor
    DESCRIPTOR_LENGTH_DEVICE,                    // bLength
    DESCRIPTOR_TYPE_DEVICE,                      // bDescriptorType
    0x00, 0x02,                                  // bcdUSB (USB 2.0)
    0,                                           // bDeviceClass
    0,                                           // bDeviceSubClass
    0,                                           // bDeviceProtocol
    ENDPOINT0_SIZE,                              // bMaxPacketSize0
    lsb(VENDOR_ID), msb(VENDOR_ID),              // idVendor
    lsb(PRODUCT_ID), msb(PRODUCT_ID),            // idProduct
    0x00, 0x01,                                  // bcdDevice (Device 1.0)
    0,                                           // iManufacturer
    0,                                           // iProduct
    0,                                           // iSerialNumber
    1,                                           // bNumConfigurations

    // Configuration descriptor
    DESCRIPTOR_LENGTH_CONFIGURATION,             // bLength;
    DESCRIPTOR_TYPE_CONFIGURATION,               // bDescriptorType;
    lsb(DESCRIPTOR_LENGTH_CONFIGURATION_TOTAL),  // wTotalLength
    msb(DESCRIPTOR_LENGTH_CONFIGURATION_TOTAL),  //
    1,                                           // bNumInterfaces
    1,                                           // bConfigurationValue
    0,                                           // iConfiguration
    0b10000000,                                  // bmAttributes
                                                 //   bit 7: reserved = 1
                                                 //   bit 6: 0 = not self-powered
                                                 //   bit 5: 0 = no remote-wakeup
                                                 //   bit 4-0: reserved = 0
    50,                                          // bMaxPower (100mA, arbitrary)

    // Interface descriptor
    DESCRIPTOR_LENGTH_INTERFACE,                 // bLength
    DESCRIPTOR_TYPE_INTERFACE,                   // bDescriptorType
    0,                                           // bInterfaceNumber
    0,                                           // bAlternateSetting
    1,                                           // bNumEndpoints
    0x03,                                        // bInterfaceClass (0x03 = HID)
    0x01,                                        // bInterfaceSubClass (0x01 = Boot)
    0x01,                                        // bInterfaceProtocol (0x01 = Keyboard)
    0,                                           // iInterface

    // HID interface descriptor
    DESCRIPTOR_LENGTH_HID_INTERFACE,             // bLength
    DESCRIPTOR_TYPE_HID_INTERFACE,               // bDescriptorType
    0x11, 0x01,                                  // bcdHID (HID 1.11)
    0,                                           // bCountryCode
    1,                                           // bNumDescriptors
    DESCRIPTOR_TYPE_HID_REPORT,                  // bDescriptorType
    lsb(DESCRIPTOR_LENGTH_HID_REPORT),           // wDescriptorLength
    msb(DESCRIPTOR_LENGTH_HID_REPORT),           //

    // Endpoint descriptor
    DESCRIPTOR_LENGTH_ENDPOINT,                  // bLength
    DESCRIPTOR_TYPE_ENDPOINT,                    // bDescriptorType
    0b10000001,                                  // bEndpointAddress
                                                 //   bit 7: 1 = IN
                                                 //   bit 6-4: reserved = 0
                                                 //   bit 3-0: 1 = endpoint number
    0b00000011,                                  // bmAttributes
                                                 //   bit 7-2: reserved = 0
                                                 //   bit 1-0: 11 = interrupt endpoint
    lsb(ENDPOINT1_SIZE), msb(ENDPOINT1_SIZE),    // wMaxPacketSize
    1,                                           // bInterval (1ms polling interval)

    // HID report descriptor (boot keyboard)
    0x05, 0x01,                                  // Usage Page (Generic Desktop)
    0x09, 0x06,                                  // Usage (Keyboard)
    0xa1, 0x01,                                  // Collection (Application)
    0x75, 0x01,                                  //   Report Size (1)
    0x95, 0x08,                                  //   Report Count (8)
    0x05, 0x07,                                  //   Usage Page (Key Codes)
    0x19, 0xe0,                                  //   Usage Minimum (224)
    0x29, 0xe7,                                  //   Usage Maximum (231)
    0x15, 0x00,                                  //   Logical Minimum (0)
    0x25, 0x01,                                  //   Logical Maximum (1)
    0x81, 0x02,                                  //   Input (Data, Variable, Absolute)
    0x95, 0x01,                                  //   Report Count (1)
    0x75, 0x08,                                  //   Report Size (8)
    0x81, 0x01,                                  //   Input (Constant)
    0x95, 0x05,                                  //   Report Count (5)
    0x75, 0x01,                                  //   Report Size (1)
    0x05, 0x08,                                  //   Usage Page (LEDs)
    0x19, 0x01,                                  //   Usage Minimum (1)
    0x29, 0x05,                                  //   Usage Maximum (5)
    0x91, 0x02,                                  //   Output (Data, Variable, Absolute)
    0x95, 0x01,                                  //   Report Count (1)
    0x75, 0x03,                                  //   Report Size (3)
    0x91, 0x01,                                  //   Output (Constant)
    0x95, 0x06,                                  //   Report Count (6)
    0x75, 0x08,                                  //   Report Size (8)
    0x15, 0x00,                                  //   Logical Minimum (0)
    0x25, 0xff,                                  //   Logical Maximum(255)
    0x05, 0x07,                                  //   Usage Page (Key Codes)
    0x19, 0x00,                                  //   Usage Minimum (0)
    0x29, 0xff,                                  //   Usage Maximum (255)
    0x81, 0x00,                                  //   Input (Data, Array)
    0xc0,                                        // End Collection
};

/*
 * Control endpoint logic
 */

// Control request constants
#define CONTROL_REQUEST_SET_ADDRESS 0x05
#define CONTROL_REQUEST_GET_DESCRIPTOR 0x06
#define CONTROL_REQUEST_SET_CONFIGURATION 0x09

// Control request type constants
#define CONTROL_DIRECTION_HOST_TO_DEVICE 0
#define CONTROL_DIRECTION_DEVICE_TO_HOST 1
#define CONTROL_TYPE_STANDARD 0
#define CONTROL_RECIPIENT_DEVICE 0

#define MASK_UDADDR 0b01111111

typedef struct {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} setup_packet_t;

static void setup_packet_recv(setup_packet_t *setup_packet)
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

// Send a descriptor
// The payload is split into packets of ENDPOINT0_SIZE bytes
static void descriptor_send(const uint8_t* data, uint16_t length)
{
    while (length)
    {
        // Prepare a data packet
        uint16_t packet_length = min(length, ENDPOINT0_SIZE);
        length -= packet_length;
        while (packet_length--)
        {
            UEDATX = pgm_read_byte(data++);
        }
        // Send the data packet
        clear_bit(UEINTX, TXINI);
        while (!read_bit(UEINTX, TXINI));
    }
    while (true)
    {
        // Send a ZLP if the host expects more data
        if (read_bit(UEINTX, TXINI))
        {
            clear_bit(UEINTX, TXINI);
        }
        // Receive the status packet and exit
        if (read_bit(UEINTX, RXOUTI))
        {
            clear_bit(UEINTX, RXOUTI);
            break;
        }
    }
}

// Send the appropriate descriptor
static void descriptor_send_dispatch(setup_packet_t *setup_packet)
{
    uint8_t descriptor_type = msb(setup_packet->wValue);
    uint8_t descriptor_index = lsb(setup_packet->wValue);

    // Device descriptor
    if (descriptor_type == DESCRIPTOR_TYPE_DEVICE &&
        descriptor_index == 0 &&
        setup_packet->wIndex == 0)
    {
        descriptor_send(
            descriptors + DESCRIPTOR_OFFSET_DEVICE,
            min(setup_packet->wLength, DESCRIPTOR_LENGTH_DEVICE));
    }

    // Configuration descriptor
    else if (
        descriptor_type == DESCRIPTOR_TYPE_CONFIGURATION &&
        descriptor_index == 0 &&
        setup_packet->wIndex == 0)
    {
        descriptor_send(
            descriptors + DESCRIPTOR_OFFSET_CONFIGURATION,
            min(setup_packet->wLength, DESCRIPTOR_LENGTH_CONFIGURATION_TOTAL));
    }

    // HID interface descriptor
    else if (
        descriptor_type == DESCRIPTOR_TYPE_HID_INTERFACE &&
        descriptor_index == 0 &&
        setup_packet->wIndex == 0)
    {
        descriptor_send(
            descriptors + DESCRIPTOR_OFFSET_HID_INTERFACE,
            min(setup_packet->wLength, DESCRIPTOR_LENGTH_HID_INTERFACE));
    }

    // HID report descriptor
    else if (
        descriptor_type == DESCRIPTOR_TYPE_HID_REPORT &&
        descriptor_index == 0 &&
        setup_packet->wIndex == 0)
    {
        descriptor_send(
            descriptors + DESCRIPTOR_OFFSET_HID_REPORT,
            min(setup_packet->wLength, DESCRIPTOR_LENGTH_HID_REPORT));
    }

    // Unknown descriptor
    else
    {
        set_bit(UECONX, STALLRQ);
    }
}

// Respond to the setup packets sent to the control endpoint
static void update_endpoint_control()
{
    // Return unless a setup packet arrived
    endpoint_select(0);
    if (!read_bit(UEINTX, RXSTPI)) return;

    // Read the setup packet
    setup_packet_t setup_packet;
    setup_packet_recv(&setup_packet);

    // Acknowledge the setup packet
    clear_bit(UEINTX, RXSTPI);

    uint8_t direction = (setup_packet.bmRequestType & 0b10000000) >> 7;
    uint8_t type      = (setup_packet.bmRequestType & 0b01100000) >> 5;
    uint8_t recipient = (setup_packet.bmRequestType & 0b00011111);

    // Get descriptor requests
    if (
        setup_packet.bRequest == CONTROL_REQUEST_GET_DESCRIPTOR &&
        direction == CONTROL_DIRECTION_DEVICE_TO_HOST &&
        type == CONTROL_TYPE_STANDARD)
    {
        descriptor_send_dispatch(&setup_packet);
    }

    // Set address requests
    else if (
        setup_packet.bRequest == CONTROL_REQUEST_SET_ADDRESS &&
        direction == CONTROL_DIRECTION_HOST_TO_DEVICE &&
        type == CONTROL_TYPE_STANDARD &&
        recipient == CONTROL_RECIPIENT_DEVICE &&
        setup_packet.wIndex == 0 &&
        setup_packet.wLength == 0)
    {
        // Set the address
        UDADDR = setup_packet.wValue & MASK_UDADDR;

        // Send the status packet
        clear_bit(UEINTX, TXINI);
        while (!read_bit(UEINTX, TXINI));

        // Enable the address
        set_bit(UDADDR, ADDEN);
    }

    // Set configuration requests
    // Take no action, the device remains configured
    else if (
        setup_packet.bRequest == CONTROL_REQUEST_SET_CONFIGURATION &&
        direction == CONTROL_DIRECTION_HOST_TO_DEVICE &&
        type == CONTROL_TYPE_STANDARD &&
        recipient == CONTROL_RECIPIENT_DEVICE &&
        setup_packet.wIndex == 0 &&
        setup_packet.wLength == 0)
    {
        // Send the status packet
        clear_bit(UEINTX, TXINI);
        while (!read_bit(UEINTX, TXINI));
    }

    // Unsupported requests
    else
    {
        set_bit(UECONX, STALLRQ);
    }
}

/*
 * Keyboard endpoint logic
 */

static struct {
    usb_report_t report;
    bool transfer_requested;
} endpoint_keyboard;

// Compare a report with the one in the endpoint
static bool report_equals(const usb_report_t *report)
{
    if (endpoint_keyboard.report.modifiers != report->modifiers)
    {
        return false;
    }
    for (uint8_t i=0; i<6; i++)
    {
        if (endpoint_keyboard.report.keys[i] != report->keys[i])
        {
            return false;
        }
    }
    return true;
}

// Copy a report to the endpoint
static void report_copy(const usb_report_t *report)
{
    endpoint_keyboard.report.modifiers = report->modifiers;
    for (uint8_t i=0; i<6; i++)
    {
        endpoint_keyboard.report.keys[i] = report->keys[i];
    }
}

// Transfer the report in the endpoint
static void report_transfer()
{
    UEDATX = endpoint_keyboard.report.modifiers;
    UEDATX = 0;
    for (uint8_t i=0; i<6; i++)
    {
        UEDATX = endpoint_keyboard.report.keys[i];
    }
}

// Send a report
// Only request a transfer to the host if the contents changed
void usb_report_send(const usb_report_t *report)
{
    if (report_equals(report)) return;
    endpoint_keyboard.transfer_requested = true;
    report_copy(report);
}

// Transfer the report when there is a pending request
// and writing on the endpoint is allowed
static void update_endpoint_keyboard()
{
    if (!endpoint_keyboard.transfer_requested) return;
    endpoint_select(1);
    if (!read_bit(UEINTX, RWAL)) return;

    report_transfer();
    clear_bit(UEINTX, FIFOCON);
    endpoint_keyboard.transfer_requested = false;
}

/*
 * USB coroutine
 */

void usb_update()
{
    update_reset();
    update_endpoint_control();
    update_endpoint_keyboard();
}
