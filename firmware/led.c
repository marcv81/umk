#include "led.h"

#include <avr/io.h>

// Declare pins B5, B6, B7 as low output
// DDR = 1, PORT = 0
void led_init()
{
    DDRB  |= 0b11100000;
    PORTB &= 0b00011111;
}

void led_1(uint8_t on)
{
    // Update pin B5
    if (on) PORTB |= 0b00100000;
    else PORTB &= 0b11011111;
}

void led_2(uint8_t on)
{
    // Update pin B6
    if (on) PORTB |= 0b01000000;
    else PORTB &= 0b10111111;
}

void led_3(uint8_t on)
{
    // Update pin B7
    if (on) PORTB |= 0b10000000;
    else PORTB &= 0b01111111;
}
