#include "matrix_left.h"

#include "i2cmaster.h"

// Column: 0   1   2   3   4   5   6
// Pin:    A0  A1  A2  A3  A2  A3  A6

// Row:    0   1   2   3   4   5
// Pin:    B5  B4  B3  B2  B1  B0

// To improve readability we handle A7 as a colum
// and B6 B7 as rows, but they are not connected

// http://ww1.microchip.com/downloads/en/DeviceDoc/22103a.pdf

#define MCP23018_ADDR    0b01000000
#define MCP23018_IODIRA  0x00
#define MCP23018_IODIRB  0x01
#define MCP23018_GPPUA   0x0c
#define MCP23018_GPPUB   0x0d
#define MCP23018_GPIOA   0x12
#define MCP23018_GPIOB   0x13

void mcp23018_write(uint8_t address, uint8_t data)
{
    i2c_start(MCP23018_ADDR | I2C_WRITE);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
}

uint8_t mcp23018_read(uint8_t address)
{
    i2c_start(MCP23018_ADDR | I2C_WRITE);
    i2c_write(address);
    i2c_start(MCP23018_ADDR | I2C_READ);
    uint8_t data = i2c_readAck();
    i2c_stop();
    return data;
}

void matrix_left_init_columns()
{
    // Disable the pull-ups
    mcp23018_write(MCP23018_GPPUA, 0b00000000);
    // Set the direction to output
    mcp23018_write(MCP23018_IODIRA, 0b00000000);
    // Set the output to hi-Z
    mcp23018_write(MCP23018_GPIOA, 0b11111111);
}

void matrix_left_init_rows()
{
    // Enable the pull-ups
    mcp23018_write(MCP23018_GPPUB, 0b11111111);
    // Set the direction to input
    mcp23018_write(MCP23018_IODIRB, 0b11111111);
}

void matrix_left_init()
{
    matrix_left_init_columns();
    matrix_left_init_rows();
}

void matrix_left_select_column(uint8_t column)
{
    // Set the selected column to low (GPIO=0)
    // Set the other columns to hi-Z (GPIO=1)
    // No update to the pull-ups or the direction (GPPU=0, IODIR=0)
    uint8_t gpioa = (column < 7) ? ~(1 << column) : 0b11111111;
    mcp23018_write(MCP23018_GPIOA, gpioa);
}

// A low input is a pressed key
// Return a uint8_t, one bit per row, a set bit is a pressed key
uint8_t matrix_left_read_rows()
{
    uint8_t data = mcp23018_read(MCP23018_GPIOB);
    return
        (data & 0b00100000 ? 0 : 0b00000001) |
        (data & 0b00010000 ? 0 : 0b00000010) |
        (data & 0b00001000 ? 0 : 0b00000100) |
        (data & 0b00000100 ? 0 : 0b00001000) |
        (data & 0b00000010 ? 0 : 0b00010000) |
        (data & 0b00000001 ? 0 : 0b00100000);
}
