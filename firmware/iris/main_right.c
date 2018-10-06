#include "config.h"
#include "i2c_slave.h"
#include "wiring.h"
#include <avr/power.h>
#include <stdbool.h>

// Called when the master reads a byte from the slave
uint8_t i2c_read(uint8_t reg_addr)
{
    // Return if the row is invalid
    if (reg_addr >= MATRIX_ROWS) return 0;

    return wiring_read_columns(reg_addr);
}

// Called when the master writes a byte to the slave
void i2c_write(uint8_t reg_addr, uint8_t data)
{
}

int main()
{
    // Bump the frequency to 16Mhz
    clock_prescale_set(clock_div_1);

    i2c_slave_init(I2C_SLAVE_ADDRESS, &i2c_read, &i2c_write);
    wiring_init();
    while (true) i2c_slave_update();
}
