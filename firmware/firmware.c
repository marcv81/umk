#include <avr/power.h>
#include <util/delay.h>

#include "led.h"
#include "matrix.h"
#include "usb_keyboard.h"
#include "i2cmaster.h"

const uint8_t key_codes[84] =
{
    0, 0,     0,     KEY_SPACE, 0,     0,     0, 0, 0,     0,     KEY_BACKSPACE, 0,          0,             0,
    0, 0,     0,     0,         0,     0,     0, 0, 0,     0,     0,             0,          0,             0,
    0, KEY_Z, KEY_X, KEY_C,     KEY_V, KEY_B, 0, 0, KEY_N, KEY_M, KEY_COMMA,     KEY_PERIOD, KEY_SLASH,     0,
    0, KEY_A, KEY_S, KEY_D,     KEY_F, KEY_G, 0, 0, KEY_H, KEY_J, KEY_K,         KEY_L,      KEY_SEMICOLON, 0,
    0, KEY_Q, KEY_W, KEY_E,     KEY_R, KEY_T, 0, 0, KEY_Y, KEY_U, KEY_I,         KEY_O,      KEY_P,         0,
    0, KEY_1, KEY_2, KEY_3,     KEY_4, KEY_5, 0, 0, KEY_6, KEY_7, KEY_8,         KEY_9,      KEY_0,         0,
};

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    led_init();
    i2c_init();
    usb_init();
    while (!usb_configured());
    _delay_ms(1000);
    matrix_init();

    // Flash each LED
    led_1(1);
    _delay_ms(100);
    led_1(0);
    led_2(1);
    _delay_ms(100);
    led_2(0);
    led_3(1);
    _delay_ms(100);
    led_3(0);

    while(1)
    {
        if (matrix_scan())
        {
            keyboard_keys[0] = 0;
            for (uint8_t i=0; i<84; i++)
            {
                if (matrix.list[i] == 1)
                {
                    keyboard_keys[0] = key_codes[i];
                    break;
                }
            }
            usb_keyboard_send();
        }
    }
}
