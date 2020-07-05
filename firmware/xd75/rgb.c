#include "rgb.h"

#include <avr/io.h>

// The WS2812 LEDs are attached to pin F6

void rgb_init() {
  DDRF |= 1 << 6;
  PORTF &= ~(1 << 6);
}

void rgb_update() {
  uint8_t counter;
  for (uint8_t i = 0; i < RGB_LEDS; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      uint8_t color = rgb_leds[i].colors[j];

      // For every bit of color the loop takes 1250ns (20 cycles)
      // 0 bit: 375ns high (6 cycles), 875ns low (14 cycles)
      // 1 bit: 875ns high (14 cycles), 375ns low (6 cycles)
      __asm__ volatile(
          "ldi %0, 8\n"
          "loop:\n"
          "sbi %2, %3\n"
          "rjmp .+0\n"
          "sbrc %1, 7\n"
          "rjmp long_wait\n"
          "cbi %2, %3\n"
          "rjmp short_wait\n"
          "long_wait:\n"
          "nop\n"
          "rjmp .+0\n"
          "short_wait:\n"
          "rjmp .+0\n"
          "rjmp .+0\n"
          "cbi %2, %3\n"
          "lsl %1\n"
          "dec %0\n"
          "brne loop\n"
          : "=&d"(counter)
          : "r"(color), "I"(_SFR_IO_ADDR(PORTF)), "I"(6));
    }
  }
}
