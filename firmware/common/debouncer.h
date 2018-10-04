#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include <stdbool.h>
#include <stdint.h>

void debouncer_init(
    void (*on_pressed)(uint8_t key),
    void (*on_released)(uint8_t key));

void debouncer_recv(uint8_t key, bool pressed);

#endif // DEBOUNCER_H
