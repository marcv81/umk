#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

#define MATRIX_ROWS 6
#define MATRIX_COLUMNS 14

// Type allowing both list and array indexing
typedef union
{
    uint8_t list[MATRIX_ROWS*MATRIX_COLUMNS];
    uint8_t array[MATRIX_ROWS][MATRIX_COLUMNS];
} matrix_t;

extern matrix_t matrix;

void matrix_init();
uint8_t matrix_scan();

#endif // MATRIX_H
