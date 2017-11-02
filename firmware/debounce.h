#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    debounce_released,
    debounce_pressed,
    debounce_unstable,
}
debounce_state_t;

void debounce_update(uint8_t row, uint8_t column, bool pressed);
debounce_state_t debounce_state(uint8_t row, uint8_t column);

#endif // DEBOUNCE_H
