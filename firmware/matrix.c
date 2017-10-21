#include "matrix.h"

#include "matrix_right.h"
#include "matrix_left.h"

matrix_t matrix;

void matrix_init()
{
    matrix_left_init();
    matrix_right_init();
}

// Update the rows for the specified column
// Return whether the matrix was updated or not
uint8_t matrix_update_rows(uint8_t rows, uint8_t column)
{
    uint8_t updated = 0;
    for (uint8_t row=0; row<MATRIX_ROWS; row++)
    {
        uint8_t pressed = (rows & (1 << row)) != 0;
        if (matrix.array[row][column] != pressed)
        {
            updated = 1;
            matrix.array[row][column] = pressed;
        }
    }
    return updated;
}

// Scan the entire matrix
// Return whether the matrix was updated or not
// Interleave left and right matrix scans
uint8_t matrix_scan()
{
    uint8_t updated = 0;
    for (uint8_t left_column=0; left_column<(MATRIX_COLUMNS/2); left_column++)
    {
        uint8_t right_column = MATRIX_COLUMNS - left_column - 1;
        // Select the column
        matrix_left_select_column(left_column);
        matrix_right_select_column(right_column);
        // Read the rows and update the matrix
        updated |= matrix_update_rows(matrix_left_read_rows(), left_column);
        updated |= matrix_update_rows(matrix_right_read_rows(), right_column);
    }
    return updated;
}
