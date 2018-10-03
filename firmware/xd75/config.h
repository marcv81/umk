#ifndef CONFIG_H
#define CONFIG_H

// Matrix size
#define MATRIX_KEYS (15 * 5)

// Cycle duration: 510us

// Number of cycles a key must look actuated to register the actuation
#define DEBOUNCER_ACTUATION_DURATION 2 // 1ms

// Number of cycles a key is ignored after an actuation
#define DEBOUNCER_COOLDOWN_DURATION 20 // 10ms

#endif // CONFIG_H
