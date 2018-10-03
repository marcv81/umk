#include "matrix.h"

#include "debouncer.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>

#define MATRIX_ROWS 5
#define MATRIX_COLUMNS 15

// Row:    0   1   2   3   4
// Pin:    D0  D1  D2  D3  D5

// Column: 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14
// Pin:    F0  F1  E6  C7  C6  B6  D4  B1  B7  B5  B4  D7  D6  B3  B0

static void init_rows()
{
    // Set the direction to input (DDR=0)
    DDRD &= 0b11010000;
    // Disable the pull-ups (PORT=0)
    PORTD &= 0b11010000;
}

static void init_columns()
{
    // Set the direction to input (DDR=0)
    DDRF &= 0b11111100;
    DDRE &= 0b10111111;
    DDRC &= 0b00111111;
    DDRB &= 0b00000100;
    DDRD &= 0b00101111;
    // Enable the pull-ups (PORT=1)
    PORTF |= 0b00000011;
    PORTE |= 0b01000000;
    PORTC |= 0b11000000;
    PORTB |= 0b11111011;
    PORTD |= 0b11010000;
}

void matrix_init()
{
    init_rows();
    init_columns();
}

static void select_row(uint8_t row)
{
    // Set all the rows to hi-Z
    // DDR=0 (input), PORT=0 (no pull-up, not updated)
    DDRD &= 0b11010000;
    // Set the selected row to low
    // DDR=1 (output), PORT=0 (low, not updated)
    switch (row)
    {
        case 0: DDRD |= 0b00000001; break;
        case 1: DDRD |= 0b00000010; break;
        case 2: DDRD |= 0b00000100; break;
        case 3: DDRD |= 0b00001000; break;
        case 4: DDRD |= 0b00100000; break;
    }
}

// A low input is a pressed key
// Return a uint16_t, one bit per column, a set bit is a pressed key
static uint16_t read_columns()
{
    return
        (PINF & 0b00000001 ? 0 : (1 <<  0)) |
        (PINF & 0b00000010 ? 0 : (1 <<  1)) |
        (PINE & 0b01000000 ? 0 : (1 <<  2)) |
        (PINC & 0b10000000 ? 0 : (1 <<  3)) |
        (PINC & 0b01000000 ? 0 : (1 <<  4)) |
        (PINB & 0b01000000 ? 0 : (1 <<  5)) |
        (PIND & 0b00010000 ? 0 : (1 <<  6)) |
        (PINB & 0b00000010 ? 0 : (1 <<  7)) |
        (PINB & 0b10000000 ? 0 : (1 <<  8)) |
        (PINB & 0b00100000 ? 0 : (1 <<  9)) |
        (PINB & 0b00010000 ? 0 : (1 << 10)) |
        (PIND & 0b10000000 ? 0 : (1 << 11)) |
        (PIND & 0b01000000 ? 0 : (1 << 12)) |
        (PINB & 0b00001000 ? 0 : (1 << 13)) |
        (PINB & 0b00000001 ? 0 : (1 << 14));
}

void matrix_update()
{
    uint8_t key = 0;
    for (uint8_t row=0; row<MATRIX_ROWS; row++)
    {
        select_row(row);
        _delay_us(1);
        uint16_t columns = read_columns();
        for (uint8_t column=0; column<MATRIX_COLUMNS; column++)
        {
            bool pressed = (columns & (1 << column));
            debouncer_feed(key++, pressed);
        }
    }
}
