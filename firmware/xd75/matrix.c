#include "matrix.h"

#include "config.h"
#include "wiring.h"

static struct { void (*send)(uint8_t key, bool pressed); } matrix;

void matrix_init(void (*send)(uint8_t key, bool pressed)) {
  wiring_init();
  matrix.send = send;
}

// Scan each key; send its state to the registered callback
void matrix_scan() {
  uint8_t key = 0;
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    uint16_t columns = wiring_read_columns(row);
    for (uint8_t column = 0; column < MATRIX_COLUMNS; column++) {
      bool pressed = (columns & (1 << column));
      matrix.send(key++, pressed);
    }
  }
}
