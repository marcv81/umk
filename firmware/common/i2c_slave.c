#include "i2c_slave.h"

#include <stdbool.h>
#include <util/twi.h>

static struct {
    // Register address
    uint8_t reg_addr;
    // Whether the register address is valid or not
    bool reg_addr_valid;
    // Read/write callbacks
    uint8_t (*read)(uint8_t reg_addr);
    void (*write)(uint8_t reg_addr, uint8_t data);
} i2c_slave;

void i2c_slave_init(
    uint8_t sla_addr,
    uint8_t (*read)(uint8_t reg_addr),
    void (*write)(uint8_t reg_addr, uint8_t data))
{
    // Set the slave address
    TWAR = (sla_addr << 1);
    // Enable the I2C bus and acknowledge any data received
    TWCR = (1 << TWEN) | (1 << TWEA);

    i2c_slave.read = read;
    i2c_slave.write = write;
}

void i2c_slave_update()
{
    // Return unless the interrupt bit is set
    if (!(TWCR & (1 << TWINT))) return;

    switch (TW_STATUS)
    {
        // Received SLA+W
        case TW_SR_SLA_ACK:
        {
            // Invalidate the register address
            i2c_slave.reg_addr_valid = false;
        }
        break;

        // Received data
        case TW_SR_DATA_ACK:
        {
            // First byte
            if (!i2c_slave.reg_addr_valid)
            {
                // Set the register address
                i2c_slave.reg_addr = TWDR;
                i2c_slave.reg_addr_valid = true;
            }
            // Subsequent bytes
            else
            {
                // Master writes a byte to the slave
                i2c_slave.write(i2c_slave.reg_addr++, TWDR);
            }
        }
        break;

        // Received SLA+R, or the transmitted data was acknowledged
        case TW_ST_SLA_ACK: case TW_ST_DATA_ACK:
        {
            // Master reads a byte from the slave
            TWDR = i2c_slave.read(i2c_slave.reg_addr++);
        }
        break;
    }

    // Clear the interrupt bit
    TWCR |= (1 << TWINT);
}
