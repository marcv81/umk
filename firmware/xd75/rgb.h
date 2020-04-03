#ifndef RGB_H
#define RGB_H

#include "config.h"
#include <stdint.h>

typedef union {
    uint8_t colors[3];
    struct {
        uint8_t green;
        uint8_t red;
        uint8_t blue;
    };
} rgb_led_t;

rgb_led_t rgb_leds[RGB_LEDS];

void rgb_init();
void rgb_update();

#endif // RGB_H
