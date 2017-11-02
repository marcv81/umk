#ifndef CONFIG_H
#define CONFIG_H

// Matrix size
#define MATRIX_ROWS 6
#define MATRIX_COLUMNS 14
#define MATRIX_KEYS (MATRIX_ROWS * MATRIX_COLUMNS)

// Keys pressed/released less than N consecutive cycles are ignored
// To define N set the N least significant bits of DEBOUNCE_MASK
#define DEBOUNCE_MASK 0b00011111 // N = 5

#endif // CONFIG_H
