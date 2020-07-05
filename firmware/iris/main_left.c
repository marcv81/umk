#include <avr/power.h>
#include <stdbool.h>

#include "core.h"

int main() {
  // Bump the frequency to 16Mhz
  clock_prescale_set(clock_div_1);

  core_init();
  while (true) core_update();
}
