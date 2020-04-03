#include "core.h"
#include <avr/power.h>
#include <stdbool.h>

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    // Disable JTAG. Set JTD twice within 4 cycles.
    MCUCR |= (1 << JTD);
    MCUCR |= (1 << JTD);

    core_init();
    while (true) core_update();
}
