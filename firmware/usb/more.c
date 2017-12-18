// Process device end-of-reset events
static void update_reset()
{
    // Return immediately if the conditions are not met
    if (!read_bit(UDINT, EORSTI)) return;

    clear_bit(UDINT, EORSTI);
    init_endpoint(0, EP_TYPE_CONTROL, EP_SIZE(ENDPOINT0_SIZE) | EP_SINGLE_BUFFER);
    usb_configuration = 0;
}

// Process update requests when writing on endpoint 1 is allowed
static void update_endpoint_1()
{
    // Return immediately if the conditions are not met
    if (!update_requested) return;
    UENUM = KEYBOARD_ENDPOINT;
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
