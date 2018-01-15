#ifndef MATRIX_H
#define MATRIX_H

#include "config.h"

#include <stdbool.h>

bool matrix_pressed[MATRIX_KEYS];

void matrix_init();
void matrix_update();

#endif // MATRIX_H
