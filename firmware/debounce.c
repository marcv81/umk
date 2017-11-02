#include "debounce.h"

#include "config.h"

static uint8_t history[MATRIX_ROWS][MATRIX_COLUMNS];

void debounce_update(uint8_t row, uint8_t column, bool pressed)
{
    // Accumulate the history: left shift
    // then set/unset the least significant bit
    history[row][column] <<= 1;
    if (pressed) history[row][column] |= 1;
}

debounce_state_t debounce_state(uint8_t row, uint8_t column)
{
    switch (history[row][column] & DEBOUNCE_MASK)
    {
        // Released for N cycles
        case 0: return debounce_released;
        // Pressed for N cycles
        case DEBOUNCE_MASK: return debounce_pressed;
        // Otherwise
        default: return debounce_unstable;
    }
}
