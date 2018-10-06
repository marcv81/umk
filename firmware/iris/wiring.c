#include "wiring.h"

#include <avr/io.h>
#include <util/delay.h>

// Row:    0   1   2   3   4
// Pin:    D7  E6  B4  D2  D4

// Column: 0   1   2   3   4   5
// Pin:    F6  F7  B1  B3  B2  B6

static void init_rows()
{
    // Set the direction to input (DDR=0)
    DDRD &= 0b01101011;
    DDRE &= 0b10111111;
    DDRB &= 0b11101111;
    // Disable the pull-ups (PORT=0)
    PORTD &= 0b01101011;
    PORTE &= 0b10111111;
    PORTB &= 0b11101111;
}

static void init_columns()
{
    // Set the direction to input (DDR=0)
    DDRF &= 0b00111111;
    DDRB &= 0b10110001;
    // Enable the pull-ups (PORT=1)
    PORTF |= 0b11000000;
    PORTB |= 0b01001110;
}

void wiring_init()
{
    init_rows();
    init_columns();
}

static void select_row(uint8_t row)
{
    // Set all the rows to hi-Z
    // DDR=0 (input), PORT=0 (no pull-up, not updated)
    DDRD &= 0b01101011;
    DDRE &= 0b10111111;
    DDRB &= 0b11101111;
    // Set the selected row to low
    // DDR=1 (output), PORT=0 (low, not updated)
    switch (row)
    {
        case 0: DDRD |= 0b10000000; break;
        case 1: DDRE |= 0b01000000; break;
        case 2: DDRB |= 0b00010000; break;
        case 3: DDRD |= 0b00000100; break;
        case 4: DDRD |= 0b00010000; break;
    }
}

// A low input is a pressed key
// Return a uint8_t, one bit per column, a set bit is a pressed key
static uint8_t read_columns()
{
    return
        (PINF & 0b01000000 ? 0 : (1 <<  0)) |
        (PINF & 0b10000000 ? 0 : (1 <<  1)) |
        (PINB & 0b00000010 ? 0 : (1 <<  2)) |
        (PINB & 0b00001000 ? 0 : (1 <<  3)) |
        (PINB & 0b00000100 ? 0 : (1 <<  4)) |
        (PINB & 0b01000000 ? 0 : (1 <<  5));
}

uint8_t wiring_read_columns(uint8_t row)
{
    select_row(row);
    _delay_us(1);
    return read_columns();
}
