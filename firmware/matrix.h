#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

// Matrix size
#define MATRIX_ROWS 6
#define MATRIX_COLUMNS 14
#define MATRIX_KEYS (MATRIX_ROWS * MATRIX_COLUMNS)

// Type allowing both list and array indexing
typedef union
{
    uint8_t list[MATRIX_KEYS];
    uint8_t array[MATRIX_ROWS][MATRIX_COLUMNS];
} matrix_t;

extern matrix_t matrix;

void matrix_init();
void matrix_update();

#endif // MATRIX_H
