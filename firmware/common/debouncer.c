#include "debouncer.h"

#include "config.h"
#include "controller.h"

static struct {
    // Cycles counter
    int8_t counter[MATRIX_KEYS];
    // Whether the debounced key is pressed or not
    bool pressed[MATRIX_KEYS];
} debouncer;

void debouncer_feed(uint8_t key, bool pressed)
{
    // If an actuation may not occur (counter < 0)
    // Increase the counter until the debouncer cools down
    if (debouncer.counter[key] < 0)
    {
        debouncer.counter[key]++;
    }

    // If an actuation may occur (counter >= 0)
    // Count how many cycles in a row the key looks actuated
    else
    {
        // If the key does not look actuated reset the counter
        if (debouncer.pressed[key] == pressed) debouncer.counter[key] = 0;

        // If the key looks actuated
        else
        {
            // Increase the cycles counter
            debouncer.counter[key]++;

            // If the key looked actuated enough cycles in a row
            if (debouncer.counter[key] == DEBOUNCER_ACTUATION_DURATION)
            {
                // Register the actuation
                if (pressed)
                {
                    debouncer.pressed[key] = true;
                    controller_on_pressed(key);
                }
                else
                {
                    debouncer.pressed[key] = false;
                    controller_on_released(key);
                }

                // Trigger the cooldown
                debouncer.counter[key] -= DEBOUNCER_COOLDOWN_DURATION;
            }
        }
    }
}
