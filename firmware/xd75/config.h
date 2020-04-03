#ifndef CONFIG_H
#define CONFIG_H

// Matrix size
#define MATRIX_ROWS 5
#define MATRIX_COLUMNS 15
#define MATRIX_KEYS (MATRIX_ROWS * MATRIX_COLUMNS)

// Cycle duration: 520us

// Number of cycles a key must look actuated to register the actuation
#define DEBOUNCER_ACTUATION_DURATION 2 // 1ms

// Number of cycles a key is ignored after an actuation
#define DEBOUNCER_COOLDOWN_DURATION 20 // 10ms

// Number of RGB LEDs
#define RGB_LEDS 6

#endif // CONFIG_H
