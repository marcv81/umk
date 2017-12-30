#define lsb(n) (n & 255)
#define msb(n) ((n >> 8) & 255)
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))

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
    msb(DESCRIPTOR_LENGTH_CONFIGURATION_TOTAL),
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
    msb(DESCRIPTOR_LENGTH_HID_REPORT),

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
    0x05, 0x01,  // Usage Page (Generic Desktop)
    0x09, 0x06,  // Usage (Keyboard)
    0xa1, 0x01,  // Collection (Application)
    0x75, 0x01,  //   Report Size (1)
    0x95, 0x08,  //   Report Count (8)
    0x05, 0x07,  //   Usage Page (Key Codes)
    0x19, 0xe0,  //   Usage Minimum (224)
    0x29, 0xe7,  //   Usage Maximum (231)
    0x15, 0x00,  //   Logical Minimum (0)
    0x25, 0x01,  //   Logical Maximum (1)
    0x81, 0x02,  //   Input (Data, Variable, Absolute)
    0x95, 0x01,  //   Report Count (1)
    0x75, 0x08,  //   Report Size (8)
    0x81, 0x01,  //   Input (Constant)
    0x95, 0x05,  //   Report Count (5)
    0x75, 0x01,  //   Report Size (1)
    0x05, 0x08,  //   Usage Page (LEDs)
    0x19, 0x01,  //   Usage Minimum (1)
    0x29, 0x05,  //   Usage Maximum (5)
    0x91, 0x02,  //   Output (Data, Variable, Absolute)
    0x95, 0x01,  //   Report Count (1)
    0x75, 0x03,  //   Report Size (3)
    0x91, 0x01,  //   Output (Constant)
    0x95, 0x06,  //   Report Count (6)
    0x75, 0x08,  //   Report Size (8)
    0x15, 0x00,  //   Logical Minimum (0)
    0x25, 0xff,  //   Logical Maximum(255)
    0x05, 0x07,  //   Usage Page (Key Codes)
    0x19, 0x00,  //   Usage Minimum (0)
    0x29, 0xff,  //   Usage Maximum (255)
    0x81, 0x00,  //   Input (Data, Array)
    0xc0,        // End Collection
};

// Send a descriptor
// The payload is split in packets of ENDPOINT0_SIZE bytes
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
    else
    if (descriptor_type == DESCRIPTOR_TYPE_CONFIGURATION &&
        descriptor_index == 0 &&
        setup_packet->wIndex == 0)
    {
        descriptor_send(
            descriptors + DESCRIPTOR_OFFSET_CONFIGURATION,
            min(setup_packet->wLength, DESCRIPTOR_LENGTH_CONFIGURATION_TOTAL));
    }

    // HID interface descriptor
    else
    if (descriptor_type == DESCRIPTOR_TYPE_HID_INTERFACE &&
        descriptor_index == 0 &&
        setup_packet->wIndex == 0)
    {
        descriptor_send(
            descriptors + DESCRIPTOR_OFFSET_HID_INTERFACE,
            min(setup_packet->wLength, DESCRIPTOR_LENGTH_HID_INTERFACE));
    }

    // HID report descriptor
    else
    if (descriptor_type == DESCRIPTOR_TYPE_HID_REPORT &&
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
