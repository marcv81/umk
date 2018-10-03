#ifndef CONFIG_H
#define CONFIG_H

// Matrix size
#define MATRIX_KEYS (15 * 5)

// Keys pressed/released less than N consecutive cycles are ignored
// To define N set the N least significant bits of DEBOUNCER_MASK
#define DEBOUNCER_MASK 0b0000001111111111 // N = 10

#endif // CONFIG_H
