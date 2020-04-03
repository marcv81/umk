#include "core.h"
#include "rgb.h"
#include <avr/power.h>
#include <stdbool.h>

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    // Disable JTAG. Set JTD twice within 4 cycles.
    MCUCR |= (1 << JTD);
    MCUCR |= (1 << JTD);

    rgb_init();
    for (int i=0; i<RGB_LEDS; i++) {
        rgb_leds[i].red = 0;
        rgb_leds[i].green = 0;
        rgb_leds[i].blue = 0;
    }
    rgb_leds[0].red = 0x1f;
    rgb_leds[1].green = 0x1f;
    rgb_leds[2].blue = 0x1f;
    rgb_leds[3].red = 0x1f;
    rgb_leds[4].green = 0x1f;
    rgb_leds[5].blue = 0x1f;
    rgb_update();

    core_init();
    while (true) core_update();
}
