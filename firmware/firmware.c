#include <avr/power.h>
#include <util/delay.h>

#include "led.h"
#include "matrix_right.h"
#include "usb_keyboard.h"

uint8_t digits[] = { KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5 };
uint8_t letters[] = { KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N };

// Scan the matrix and type debug data over the USB keyboard
void test_matrix()
{
    for (uint8_t column=0; column<7; column++)
    {
        matrix_right_select_column(column + 7);
        uint8_t rows = matrix_right_read_rows();
        for (uint8_t row=0; row<6; row++)
        {
            if (rows & (1 << row))
            {
                usb_keyboard_press(letters[column + 7], 0);
                usb_keyboard_press(digits[row], 0);
            }
        }
    }
}

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    led_init();

    usb_init();
    while (!usb_configured()) /* wait */ ;
    _delay_ms(1000);

    matrix_right_init();

    // Turn each LED on
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
        test_matrix();
    }
}
