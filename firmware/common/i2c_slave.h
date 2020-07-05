#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <stdint.h>

void i2c_slave_init(uint8_t sla_addr, uint8_t (*read)(uint8_t reg_addr),
                    void (*write)(uint8_t reg_addr, uint8_t data));

void i2c_slave_update();

#endif  // I2C_SLAVE_H
