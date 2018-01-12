#include <util/twi.h>

#include "i2c.h"

#include "config.h"

void i2c_init()
{
    // Clear the prescaler bits in TWSR
    TWSR &= 0b11111100;
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

// Prepare to write to a slave
static bool sla_w(uint8_t sla_addr)
{
    TWDR = sla_addr << 1;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return (TW_STATUS == TW_MT_SLA_ACK);
}

// Prepare to read from a slave
static bool sla_r(uint8_t sla_addr)
{
    TWDR = (sla_addr << 1) | 1;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return (TW_STATUS == TW_MR_SLA_ACK);
}

// Write one byte to a slave
static bool data_w(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return (TW_STATUS == TW_MT_DATA_ACK);
}

// Read one byte from a slave
// There are more bytes in this transfer
static bool data_r_ack(uint8_t *data)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    *data = TWDR;
    return (TW_STATUS == TW_MR_DATA_ACK);
}

// Read one byte from a slave
// This is the last byte in this transfer
static bool data_r_nack(uint8_t *data)
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    *data = TWDR;
    return (TW_STATUS == TW_MR_DATA_NACK);
}

bool i2c_write_byte(uint8_t sla_addr, uint8_t reg_addr, uint8_t data)
{
    start();
    if (!sla_w(sla_addr)) goto error;
    if (!data_w(reg_addr)) goto error;
    if (!data_w(data)) goto error;
    stop();
    return true;

error:
    stop();
    return false;
}

bool i2c_read_byte(uint8_t sla_addr, uint8_t reg_addr, uint8_t *data)
{
    start();
    if (!sla_w(sla_addr)) goto error;
    if (!data_w(reg_addr)) goto error;
    start();
    if (!sla_r(sla_addr)) goto error;
    if (!data_r_nack(data)) goto error;
    stop();
    return true;

error:
    stop();
    return false;
}
