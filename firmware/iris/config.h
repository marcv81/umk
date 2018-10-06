#ifndef CONFIG_H
#define CONFIG_H

// Matrix size
#define MATRIX_ROWS 5
#define MATRIX_COLUMNS 12
#define MATRIX_KEYS (MATRIX_ROWS * MATRIX_COLUMNS)

// I2C slave address
#define I2C_SLAVE_ADDRESS 0x88

// I2C bus frequency
#define I2C_FREQUENCY 400000

// Cycle duration: 540us

// Number of cycles a key must look actuated to register the actuation
#define DEBOUNCER_ACTUATION_DURATION 2 // 1ms

// Number of cycles a key is ignored after an actuation
#define DEBOUNCER_COOLDOWN_DURATION 18 // 10ms

#endif // CONFIG_H
