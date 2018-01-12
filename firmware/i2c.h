#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

void i2c_init();

bool i2c_write_byte(uint8_t sla_addr, uint8_t reg_addr, uint8_t data);
bool i2c_read_byte(uint8_t sla_addr, uint8_t reg_addr, uint8_t *data);

#endif // I2C_H
