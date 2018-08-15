#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stdint.h>

bool matrix_pressed(uint8_t key);

void matrix_init();
void matrix_update();

#endif // MATRIX_H
