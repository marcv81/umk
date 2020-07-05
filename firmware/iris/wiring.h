#ifndef WIRING_H
#define WIRING_H

#include <stdint.h>

void wiring_init();
uint8_t wiring_read_columns(uint8_t row);

#endif  // WIRING_H
