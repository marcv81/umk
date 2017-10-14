#include "led.h"

#include <avr/io.h>

// Declare pins B5, B6, B7 as output
void init_leds() { DDRB |= 0b11100000; }

void led_1(int on)
{
    // Update pin B5
    if (on) PORTB |= 0b00100000;
    else PORTB &= 0b11011111;
}

void led_2(int on)
{
    // Update pin B6
    if (on) PORTB |= 0b01000000;
    else PORTB &= 0b10111111;
}

void led_3(int on)
{
    // Update pin B7
    if (on) PORTB |= 0b10000000;
    else PORTB &= 0b01111111;
}
