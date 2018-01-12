#include "matrix.h"

#include "config.h"
#include "matrix_right.h"
#include "matrix_left.h"
#include "debounce.h"

static union {
    bool list[MATRIX_KEYS];
    bool array[MATRIX_ROWS][MATRIX_COLUMNS];
} pressed;

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
        bool key_pressed = (rows & (1 << row));
        debounce_update(row, column, key_pressed);
        switch(debounce_state(row, column))
        {
            case debounce_released:
                pressed.array[row][column] = false;
                break;
            case debounce_pressed:
                pressed.array[row][column] = true;
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

// Return whether a key is pressed or not
bool matrix_pressed(uint8_t key)
{
    return pressed.list[key];
}
