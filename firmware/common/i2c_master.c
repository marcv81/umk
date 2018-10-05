#include "i2c_master.h"

#include "config.h"
#include <util/twi.h>

#define TWSR_PRESCALER_MASK 0b11111100

void i2c_master_init()
{
    // Clear the prescaler bits in TWSR
    TWSR &= TWSR_PRESCALER_MASK;
    // Set TWBR to achieve the specified I2C frequency
    TWBR = ((F_CPU / I2C_FREQUENCY) - 16) / 2;
}

// Send a start (or repeated start) condition
static void start()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    while (!(TWCR & (1 << TWINT)));
}

// Send a stop condition
static void stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    while (TWCR & (1 << TWSTO));
}

// Prepare to write to a slave (send SLA+W)
static void sla_w(uint8_t sla_addr)
{
    TWDR = sla_addr << 1;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

// Prepare to read from a slave (send SLA+R)
static void sla_r(uint8_t sla_addr)
{
    TWDR = (sla_addr << 1) | 1;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

// Write one byte to a slave
static void data_w(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

// Read one byte from a slave
// There are more bytes in this transfer
static uint8_t data_r_ack()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

// Read one byte from a slave
// This is the last byte in this transfer
static uint8_t data_r_nack()
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void i2c_master_write(
    uint8_t sla_addr, uint8_t reg_addr, uint8_t data[], uint8_t len)
{
    start();
    sla_w(sla_addr);
    data_w(reg_addr);
    for (uint8_t i=0; i<len; i++)
    {
        data_w(data[i]);
    }
    stop();
}

void i2c_master_read(
    uint8_t sla_addr, uint8_t reg_addr, uint8_t data[], uint8_t len)
{
    start();
    sla_w(sla_addr);
    data_w(reg_addr);
    start();
    sla_r(sla_addr);
    for (uint8_t i=0; i<len-1; i++)
    {
        data[i] = data_r_ack();
    }
    data[len - 1] = data_r_nack();
    stop();
}
