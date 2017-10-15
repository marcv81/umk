#include <avr/power.h>
#include <util/delay.h>

#include "led.h"
#include "usb_keyboard.h"

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    init_leds();

    usb_init();
    while (!usb_configured()) /* wait */ ;
    _delay_ms(1000);

    while(1)
    {
        led_3(0);
        led_1(1);
        usb_keyboard_press(KEY_A, 0);
        _delay_ms(500);
        led_1(0);
        led_2(1);
        usb_keyboard_press(KEY_B, 0);
        _delay_ms(500);
        led_2(0);
        led_3(1);
        usb_keyboard_press(KEY_C, 0);
        _delay_ms(500);
    }
}
