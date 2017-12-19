#define DIRECTION_HOST_TO_DEVICE 0
#define DIRECTION_DEVICE_TO_HOST 1
#define TYPE_STANDARD 0
#define RECIPIENT_DEVICE 0

#define MASK_UDADDR 0b01111111

// Process device end-of-reset events
static void update_reset()
{
    // Return immediately if the conditions are not met
    if (!read_bit(UDINT, EORSTI)) return;

    clear_bit(UDINT, EORSTI);
    init_endpoint(0, ENDPOINT0_CFG0, ENDPOINT0_CFG1);
    init_endpoint(1, ENDPOINT1_CFG0, ENDPOINT1_CFG1);
}

// Process endpoint 0 setup packets
static void update_endpoint_0()
{
    // Return immediately if the conditions are not met
    select_endpoint(0);
    if (!read_bit(UEINTX, RXSTPI)) return;

    // Read the setup packet
    setup_packet_t setup_packet;
    recv_setup_packet(&setup_packet);
    clear_bit(UEINTX, RXSTPI);

    uint8_t direction = (setup_packet.bmRequestType & 0b10000000) >> 7;
    uint8_t type      = (setup_packet.bmRequestType & 0b01100000) >> 5;
    uint8_t recipient = (setup_packet.bmRequestType & 0b00011111);

    // Get descriptor requests
    if (setup_packet.bRequest == GET_DESCRIPTOR &&
        direction == DIRECTION_DEVICE_TO_HOST &&
        type == TYPE_STANDARD)
    {
        descriptor_send_dispatch(&setup_packet);
    }

    // Set address requests
    else
    if (setup_packet.bRequest == SET_ADDRESS &&
        direction == DIRECTION_HOST_TO_DEVICE &&
        type == TYPE_STANDARD &&
        recipient == RECIPIENT_DEVICE &&
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
    else
    if (setup_packet.bRequest == SET_CONFIGURATION &&
        direction == DIRECTION_HOST_TO_DEVICE &&
        type == TYPE_STANDARD &&
        recipient == RECIPIENT_DEVICE &&
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

// Process update requests when writing on endpoint 1 is allowed
static void update_endpoint_1()
{
    // Return immediately if the conditions are not met
    if (!update_requested) return;
    select_endpoint(1);
    if (!read_bit(UEINTX, RWAL)) return;

    send_keyboard_report(&usb_keyboard_report);
    clear_bit(UEINTX, FIFOCON);
    update_requested = false;
}

void usb_update()
{
    update_reset();
    update_endpoint_0();
    update_endpoint_1();
}
