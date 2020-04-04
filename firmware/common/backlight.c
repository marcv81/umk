#include "backlight.h"

#include "config.h"
#include "rgb.h"
#include <avr/pgmspace.h>

static struct {
    // Cycle counter to manage the frames duration
    uint8_t cycle;
    // Frame counter
    uint8_t frame;
} backlight;

// Precalculated first quadrant of 120 * sine
static const PROGMEM uint8_t sine_table[65] = {
    0, 3, 6, 9, 12, 15, 18, 21, 23, 26, 29, 32, 35, 38, 40, 43, 46, 49, 51,
    54, 57, 59, 62, 64, 67, 69, 71, 74, 76, 78, 81, 83, 85, 87, 89, 91, 93,
    95, 96, 98, 100, 101, 103, 104, 106, 107, 108, 110, 111, 112, 113, 114,
    115, 116, 116, 117, 118, 118, 119, 119, 119, 120, 120, 120, 120,
};

// Sine of an angle, input in [0:255], output in [-120:120]
static int8_t sine(uint8_t angle)
{
    if (angle < 128)
    {
        uint8_t index = (angle < 64) ? angle : 128 - angle;
        return pgm_read_byte(&sine_table[index]);
    }
    else
    {
        uint8_t index = (angle < 192) ? angle - 128 : 256 - angle;
        return -pgm_read_byte(&sine_table[index]);
    }
}

static void breathe()
{
    uint8_t amplitude = 135 + sine(backlight.frame);
    for (int i=0; i<RGB_LEDS; i++)
    {
        rgb_leds[i].red = amplitude;
        rgb_leds[i].green = 0;
        rgb_leds[i].blue = 0;
    }
}

void backlight_init()
{
    rgb_init();
    backlight.cycle = 0;
    backlight.frame = 63;
}

void backlight_update()
{
    if (backlight.cycle-- == 0)
    {
        backlight.cycle = BACKLIGHT_FRAME_DURATION;
        backlight.frame++;
        breathe();
        rgb_update();
    }
}
