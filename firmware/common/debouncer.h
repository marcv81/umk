#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include <stdbool.h>
#include <stdint.h>

void debouncer_feed(uint8_t key, bool pressed);

#endif // DEBOUNCER_H
