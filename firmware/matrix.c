#include "matrix.h"

#include "matrix_right.h"
#include "matrix_left.h"

// Key states maintained less than N cycles are ignored
// To define N set the N least significant bits of DEBOUNCE_MASK
#define DEBOUNCE_MASK 0b00011111 // N = 5

matrix_t matrix;

static matrix_t history;

void matrix_init()
{
    matrix_left_init();
    matrix_right_init();
}

// Update the rows for the specified column
static void update_rows(uint8_t rows, uint8_t column)
{
    for (uint8_t row=0; row<MATRIX_ROWS; row++)
    {
        // Accumulate the history: left shift the history and set
        // the least significant bit according to the current state
        uint8_t pressed = (rows & (1 << row)) != 0;
        history.array[row][column] <<= 1;
        history.array[row][column] |= pressed;

        // Check the history during the N past cycles
        switch (history.array[row][column] & DEBOUNCE_MASK)
        {
            // The key was consistently released
            case 0:
                matrix.array[row][column] = 0;
                break;
            // The key was consistently pressed
            case DEBOUNCE_MASK:
                matrix.array[row][column] = 1;
                break;
        }
    }
}

// Update the entire matrix
// Interleave left and right matrix scans
void matrix_update()
{
    for (uint8_t left_column=0; left_column<(MATRIX_COLUMNS/2); left_column++)
    {
        uint8_t right_column = MATRIX_COLUMNS - left_column - 1;
        // Select the column
        matrix_left_select_column(left_column);
        matrix_right_select_column(right_column);
        // Read the rows and update the matrix
        update_rows(matrix_left_read_rows(), left_column);
        update_rows(matrix_right_read_rows(), right_column);
    }
}
