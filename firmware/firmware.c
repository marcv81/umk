#include <avr/power.h>
#include <util/delay.h>

#include "led.h"

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);
    init_leds();
    while(1)
    {
        led_3(0);
        led_1(1);
        _delay_ms(500);
        led_1(0);
        led_2(1);
        _delay_ms(500);
        led_2(0);
        led_3(1);
        _delay_ms(500);
    }
}
