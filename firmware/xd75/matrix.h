#ifndef MATRIX_H
#define MATRIX_H

#include "config.h"
#include <stdbool.h>

void matrix_init();
void matrix_update();

bool matrix_pressed[MATRIX_KEYS];

#endif // MATRIX_H
