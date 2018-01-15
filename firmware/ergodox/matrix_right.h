#ifndef MATRIX_RIGHT_H
#define MATRIX_RIGHT_H

#include <stdint.h>

void matrix_right_init();
void matrix_right_select_column(uint8_t column);
uint8_t matrix_right_read_rows();

#endif // MATRIX_RIGHT_H
