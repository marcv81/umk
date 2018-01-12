#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c_init();

void i2c_write_byte(uint8_t sla_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c_read_byte(uint8_t sla_addr, uint8_t reg_addr);

#endif // I2C_H
