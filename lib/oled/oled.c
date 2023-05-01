#include <stdio.h>
#include <oled.h>
#include <i2c.h>
#include "driver/gpio.h"

void startCondition(uint32_t clock_pin, uint32_t data_pin);
void endCondition(uint32_t clock_pin, uint32_t data_pin);

OLedError writeAndReadAck(uint8_t *data, int bits);
OLedError setupCommand();
OLedError setupData();

OLedError startDisplay(uint32_t clock_pin, uint32_t data_pin)
{
    uint8_t err = setup_i2c(clock_pin, data_pin);
    if (err)
    {
        return OLED_I2C_ERROR;
    }

    // Start condition
    gpio_set_direction(clock_pin, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(data_pin, GPIO_MODE_DEF_OUTPUT);
    startCondition(clock_pin, data_pin);

    uint8_t buffer[1], err;

    // TODO Create a function that do both write and read ack

    // Slave address and ack bit
    err = write_i2c(OLED_ADDR, 8);

    if (err)
    {
        return err;
    }

    read_i2c(buffer, 1);

    if (!buffer[0])
    {
        return OLED_I2C_ERROR;
    }

    // Command byte
    err = write_i2c(OLED_COMMAND_BYTE, 8);

    if (err)
    {
        return err;
    }

    read_i2c(buffer, 1);

    if (!buffer[0])
    {
        return OLED_I2C_ERROR;
    }

    // Turn monitor on
    err = write_i2c(OLED_TURN_ON, 8);

    if (err)
    {
        return err;
    }

    read_i2c(buffer, 1);

    if (!buffer[0])
    {
        return OLED_I2C_ERROR;
    }

    // End condition
    endCondition(clock_pin, data_pin);

    return OLED_I2C_OK;
}

OLedError updateDisplay(uint32_t clock_pin, uint32_t data_pin, uint8_t *data)
{
    uint8_t err;
    startCondition(clock_pin, data_pin); // How to remove this guy from here?

    err = setupCommand();
    if (err)
        return err;

    err = writeAndReadAck(OLED_PAGE_ADDRESSING_MODE, 8);
    if (err)
        return err;

    // TODO: Setup initial page and initial column?

    endCondition(clock_pin, data_pin);

    startCondition(clock_pin, data_pin);
    err = setupData();
    if (err)
        return err;

    // Data will be stored in GDDRAM, see SSD1306 documentation to see why is necessary to write like this.
    for (uint8_t page = 0; page < 8; page++)
    {
        // Setup page
        startCondition(clock_pin, data_pin);
        endCondition(clock_pin, data_pin);

        // Column index is auto-updated
        for (uint8_t column = 0; column < 128; column++)
        {
            // We will need to set fisrt 8 bis of row and column
        }

    }

    endCondition(clock_pin, data_pin);

    return OLED_I2C_OK;
}

OLedError setDisplayFullOn(uint32_t clock_pin, uint32_t data_pin)
{
    uint8_t err;
    startCondition(clock_pin, data_pin);

    err = setupCommand();
    if (err)
    {
        return err;
    }

    err = writeAndReadAck(OLED_ALL_ON_DISPLAY, 8);
    if (err)
    {
        return err;
    }

    endCondition(clock_pin, data_pin);

    return OLED_I2C_OK;
}

OLedError setDisplayRAMMode(uint32_t clock_pin, uint32_t data_pin)
{
    uint8_t err;
    startCondition(clock_pin, data_pin);

    // Slave address and ack bit
    err = setupCommand();
    if (err)
    {
        return err;
    }

    err = writeAndReadAck(OLED_RAM_DISPLAY, 8);
    if (err)
    {
        return err;
    }

    endCondition(clock_pin, data_pin);

    return OLED_I2C_OK;
}

void startCondition(uint32_t clock_pin, uint32_t data_pin)
{
    // Data pin shound be used with a Pullup resistor
    gpio_set_level(data_pin, 1);
    gpio_set_level(clock_pin, 1);
    gpio_set_level(data_pin, 0);
}

void endCondition(uint32_t clock_pin, uint32_t data_pin)
{
    // Data pin shound be used with a Pullup resistor
    gpio_set_level(data_pin, 0);
    gpio_set_level(clock_pin, 1);
    gpio_set_level(data_pin, 1);
}

OLedError writeAndReadAck(uint8_t *data, int bits)
{
    uint8_t buffer[1], err;

    err = write_i2c(OLED_ADDR, 8);
    if (err)
    {
        return err;
    }

    err = read_i2c(buffer, 1);

    if (!buffer[0] || err)
    {
        return OLED_I2C_ERROR;
    }
}

OLedError setupCommand()
{
    uint8_t err;

    // Slave address and ack bit
    err = writeAndReadAck(OLED_ADDR, 8);
    if (err)
    {
        return err;
    }

    // Command byte
    err = writeAndReadAck(OLED_COMMAND_BYTE, 8);
    if (err)
    {
        return err;
    }
}

OLedError setupData()
{
    uint8_t err;

    // Slave address and ack bit
    err = writeAndReadAck(OLED_ADDR, 8);
    if (err)
    {
        return err;
    }

    // Command byte
    err = writeAndReadAck(OLED_DATA_BYTE, 8);
    if (err)
    {
        return err;
    }
}