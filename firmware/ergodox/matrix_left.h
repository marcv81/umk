#ifndef MATRIX_LEFT_H
#define MATRIX_LEFT_H

#include <stdint.h>

void matrix_left_init();
void matrix_left_select_column(uint8_t column);
uint8_t matrix_left_read_rows();

#endif // MATRIX_LEFT_H
