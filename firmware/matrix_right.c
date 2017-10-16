#include "matrix_right.h"

#include <avr/io.h>

// Column: 7   8   9   10  11  12  13
// Pin:    B0  B1  B2  B3  D2  D3  C6

// Row:    0   1   2   3   4   5
// Pin:    F7  F6  F5  F4  F1  F0

void matrix_right_init_columns()
{
    // Set the direction to input (DDR=0)
    DDRB &= 0b11110000;
    DDRD &= 0b11110011;
    DDRC &= 0b10111111;
    // Disable the pull-ups (PORT=0)
    PORTB &= 0b11110000;
    PORTD &= 0b11110011;
    PORTC &= 0b10111111;
}

void matrix_right_init_rows()
{
    // Set the direction to input (DDR=0)
    DDRF &= 0b00001100;
    // Enable the pull-ups (PORT=1)
    PORTF |= 0b11110011;
}

void matrix_right_init()
{
    matrix_right_init_columns();
    matrix_right_init_rows();
}

void matrix_right_select_column(uint8_t column)
{
    // Set all the columns to hi-Z
    // DDR=0 (input), PORT=0 (no pull-up, not updated)
    DDRB &= 0b11110000;
    DDRD &= 0b11110011;
    DDRC &= 0b10111111;
    // Set the selected column to low
    // DDR=1 (output), PORT=0 (low, not updated)
    switch(column)
    {
        case 7:  DDRB |= 0b00000001; break;
        case 8:  DDRB |= 0b00000010; break;
        case 9:  DDRB |= 0b00000100; break;
        case 10: DDRB |= 0b00001000; break;
        case 11: DDRD |= 0b00000100; break;
        case 12: DDRD |= 0b00001000; break;
        case 13: DDRC |= 0b01000000; break;
    }
}

// A low input is a pressed key
// Return a uint8_t, one bit per row, a set bit is a pressed key
uint8_t matrix_right_read_rows()
{
    return
        (PINF & 0b10000000 ? 0 : 0b00000001) |
        (PINF & 0b01000000 ? 0 : 0b00000010) |
        (PINF & 0b00100000 ? 0 : 0b00000100) |
        (PINF & 0b00010000 ? 0 : 0b00001000) |
        (PINF & 0b00000010 ? 0 : 0b00010000) |
        (PINF & 0b00000001 ? 0 : 0b00100000);
}
