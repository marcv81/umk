#include "debouncer.h"

#include "config.h"

static struct {
  // Cycles counter
  int8_t counter[MATRIX_KEYS];
  // Whether the debounced key is pressed or not
  bool pressed[MATRIX_KEYS];
  // Actuation callbacks
  void (*on_pressed)(uint8_t key);
  void (*on_released)(uint8_t key);
} debouncer;

void debouncer_init(void (*on_pressed)(uint8_t key),
                    void (*on_released)(uint8_t key)) {
  debouncer.on_pressed = on_pressed;
  debouncer.on_released = on_released;
}

void debouncer_recv(uint8_t key, bool pressed) {
  // If an actuation may not occur (counter < 0)
  // Increase the counter until the debouncer cools down
  if (debouncer.counter[key] < 0) {
    debouncer.counter[key]++;
  }

  // If an actuation may occur (counter >= 0)
  // Count how many cycles in a row the key looks actuated
  else {
    // If the key does not look actuated reset the counter
    if (debouncer.pressed[key] == pressed) debouncer.counter[key] = 0;

    // If the key looks actuated
    else {
      // Increase the cycles counter
      debouncer.counter[key]++;

      // If the key looked actuated enough cycles in a row
      if (debouncer.counter[key] == DEBOUNCER_ACTUATION_DURATION) {
        // Register the actuation
        if (pressed) {
          debouncer.pressed[key] = true;
          debouncer.on_pressed(key);
        } else {
          debouncer.pressed[key] = false;
          debouncer.on_released(key);
        }

        // Trigger the cooldown
        debouncer.counter[key] -= DEBOUNCER_COOLDOWN_DURATION;
      }
    }
  }
}
