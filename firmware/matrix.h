#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdbool.h>

void matrix_init();
void matrix_update();
bool matrix_pressed(uint8_t key);

#endif // MATRIX_H
