#include <avr/power.h>
#include <util/delay.h>

#include "led.h"
#include "matrix_right.h"
#include "matrix_left.h"
#include "usb_keyboard.h"
#include "i2cmaster.h"
#include "millis.h"

uint8_t digits[] = { KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9 };
uint8_t letters[] = { KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N };

void debug_decimal(uint16_t value)
{
    char buffer[10];
    itoa(value, buffer, 10);
    for (uint8_t i=0; i<10; i++)
    {
        if (buffer[i] == 0) break;
        usb_keyboard_press(digits[buffer[i] - '0'], 0);
        _delay_ms(50);
    }
}

// Scan the matrix and type debug data over the USB keyboard
void test_matrix()
{
    for (uint8_t column=0; column<7; column++)
    {
        uint8_t rows;
        matrix_right_select_column(column + 7);
        matrix_left_select_column(column);
        rows = matrix_right_read_rows();
        for (uint8_t row=0; row<6; row++)
        {
            if (rows & (1 << row))
            {
                usb_keyboard_press(letters[column + 7], 0);
                usb_keyboard_press(digits[row], 0);
            }
        }
        rows = matrix_left_read_rows();
        for (uint8_t row=0; row<6; row++)
        {
            if (rows & (1 << row))
            {
                usb_keyboard_press(letters[column], 0);
                usb_keyboard_press(digits[row], 0);
            }
        }
    }
}

int main()
{
    millis_t start, stop;

    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    led_init();
    millis_init();

    i2c_init();

    usb_init();
    while (!usb_configured());

    // Should type 1000, more or less
    start = millis_get();
    _delay_ms(1000);
    stop = millis_get();
    debug_decimal(stop - start);

    matrix_right_init();
    matrix_left_init();

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
        start = millis_get();
        for (uint16_t i=0; i<1000; i++)
        {
            test_matrix();
        }
        stop = millis_get();
        debug_decimal(stop - start);
    }
}
