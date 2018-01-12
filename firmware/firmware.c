#include <avr/power.h>
#include <util/delay.h>

#include "led.h"
#include "matrix.h"
#include "usb.h"
#include "i2c.h"
#include "controller.h"

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    // Initialize the LEDs
    led_init();

    // LEDs on
    led_1(true);
    _delay_ms(100);
    led_2(true);
    _delay_ms(100);
    led_3(true);

    // Initialize everything else
    i2c_init();
    matrix_init();
    usb_init();

    // LEDs off
    led_1(false);
    _delay_ms(100);
    led_2(false);
    _delay_ms(100);
    led_3(false);

    // Main loop
    while (true)
    {
        matrix_update();
        controller_update();
        usb_update();
    }
}
