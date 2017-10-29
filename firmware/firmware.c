#include <avr/power.h>
#include <util/delay.h>

#include "led.h"
#include "matrix.h"
#include "usb_keyboard.h"
#include "i2cmaster.h"
#include "controller.h"

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    led_init();
    led_1(1);
    led_2(1);
    led_3(1);
    _delay_ms(200);

    i2c_init();
    led_1(0);

    matrix_init();
    led_2(0);

    usb_init();
    while (!usb_configured());
    _delay_ms(1000);
    led_3(0);

    while(1)
    {
        matrix_update();
        controller_update();
    }
}
