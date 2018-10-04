#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stdint.h>

void matrix_init(void (*send)(uint8_t key, bool pressed));
void matrix_scan();

#endif // MATRIX_H
