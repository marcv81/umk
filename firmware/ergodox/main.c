#include "controller.h"
#include "led.h"

#include <stdbool.h>

#include <avr/power.h>
#include <util/delay.h>

#define DELAY_LED 100

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    // Initialize the LEDs
    led_init();

    // LEDs on
    led_1(true);
    _delay_ms(DELAY_LED);
    led_2(true);
    _delay_ms(DELAY_LED);
    led_3(true);

    // Initialize everything
    controller_init();

    // LEDs off
    led_1(false);
    _delay_ms(DELAY_LED);
    led_2(false);
    _delay_ms(DELAY_LED);
    led_3(false);

    // Main loop
    while (true) controller_update();
}
