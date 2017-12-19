#define DESCRIPTOR_TYPE_DEVICE 0x01
#define DESCRIPTOR_TYPE_CONFIGURATION 0x02
#define DESCRIPTOR_TYPE_HID_INTERFACE 0x21
#define DESCRIPTOR_TYPE_HID_REPORT 0x22

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))

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
    uint8_t descriptor_type = MSB(setup_packet->wValue);
    uint8_t descriptor_index = LSB(setup_packet->wValue);

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
            min(setup_packet->wLength, DESCRIPTOR_LENGTH_CONFIGURATION));
    }

    // HID interface descriptor
    else
    if (descriptor_type == DESCRIPTOR_TYPE_HID_INTERFACE &&
        descriptor_index == 0 &&
        setup_packet->wIndex == KEYBOARD_INTERFACE)
    {
        descriptor_send(
            descriptors + DESCRIPTOR_OFFSET_HID_INTERFACE,
            min(setup_packet->wLength, DESCRIPTOR_LENGTH_HID_INTERFACE));
    }

    // HID report descriptor
    else
    if (descriptor_type == DESCRIPTOR_TYPE_HID_REPORT &&
        descriptor_index == 0 &&
        setup_packet->wIndex == KEYBOARD_INTERFACE)
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
