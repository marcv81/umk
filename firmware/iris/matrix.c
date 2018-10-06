#include "matrix.h"

#include "config.h"
#include "i2c_master.h"
#include "wiring.h"

static struct {
    void (*send)(uint8_t key, bool pressed);
} matrix;

void matrix_init(void (*send)(uint8_t key, bool pressed))
{
    i2c_master_init();
    wiring_init();
    matrix.send = send;
}

// Scan each key; send its state to the registered callback
void matrix_scan()
{
    uint8_t slave_columns[MATRIX_ROWS];
    i2c_master_read(I2C_SLAVE_ADDRESS, 0, slave_columns, MATRIX_ROWS);

    uint8_t key = 0;
    for (uint8_t row=0; row<MATRIX_ROWS; row++)
    {
        uint8_t columns;

        // Left side (master)
        columns = wiring_read_columns(row);
        for (uint8_t column=0; column<MATRIX_COLUMNS/2; column++)
        {
            bool pressed = (columns & (1 << column));
            matrix.send(key++, pressed);
        }

        // Right side (slave), which mirrors the left side
        columns = slave_columns[row];
        for (uint8_t column=0; column<MATRIX_COLUMNS/2; column++)
        {
            uint8_t mirror_column = MATRIX_COLUMNS/2 - 1 - column;
            bool pressed = (columns & (1 << mirror_column));
            matrix.send(key++, pressed);
        }
    }
}
