#include "controller.h"

#include <stdbool.h>

#include <avr/power.h>

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    // Initialize everything
    controller_init();

    // Main loop
    while (true) controller_update();
}
