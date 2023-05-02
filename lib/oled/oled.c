#include <stdio.h>
#include <oled.h>
#include <i2c.h>
#include "driver/gpio.h"
#include <debug.h>

const OLedError OLED_OUT_OF_BOUNDS = 12;
const OLedError OLED_I2C_ERROR = 13;
const OLedError OLED_I2C_OK = 0;

void startCondition(uint32_t clock_pin, uint32_t data_pin);
void endCondition(uint32_t clock_pin, uint32_t data_pin);

OLedError sendCommand(uint32_t clock_pin, uint32_t data_pin, uint8_t command);
OLedError writeByteAndReadAck(uint8_t data);
OLedError setupCommand();
OLedError setupData();

OLedError startDisplay(uint32_t clock_pin, uint32_t data_pin)
{
    debug("Starting Display...\n");
    uint8_t err = setup_i2c(clock_pin, data_pin);
    if (err)
    {
        return OLED_I2C_ERROR;
    }

    gpio_set_direction(clock_pin, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(data_pin, GPIO_MODE_DEF_OUTPUT);

    return sendCommand(clock_pin, data_pin, OLED_TURN_ON);
}

OLedError updateDisplay(uint32_t clock_pin, uint32_t data_pin, uint8_t *data) // Data is OLED_HEIGHT * OLED_WIDTH
{
    uint8_t err, value;
    int row, index;
    startCondition(clock_pin, data_pin); // How to remove this guy from here?

    err = setupCommand();
    if (err)
        return err;

    err = writeByteAndReadAck(OLED_PAGE_ADDRESSING_MODE);
    if (err)
        return err;

    // TODO: Setup initial page and initial column?

    endCondition(clock_pin, data_pin);

    startCondition(clock_pin, data_pin);
    err = setupData();
    if (err)
    {
        return err;
    }

    // Data will be stored in GDDRAM, see SSD1306 documentation to see why is necessary to write like this.
    for (uint8_t page = 0; page < 8; page++)
    {
        // Setup page
        startCondition(clock_pin, data_pin);
        endCondition(clock_pin, data_pin);

        // Column index is auto-updated
        for (uint8_t column = 0; column < 128; column++)
        {
            // We will need to set first 1 byte of row and column and read the ack
            uint8_t byte = 0;
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                row = (page * 8) + bit;
                index = row * OLED_WIDTH + column;
                value = data[index];
                byte = (byte << bit) | (value & 0x1); // Adding (value & 0x1) to force 1 or 0. Could also use value >=1
            }

            err = writeByteAndReadAck(byte);
            if (err)
            {
                return err;
            }
        }
    }

    endCondition(clock_pin, data_pin);

    return OLED_I2C_OK;
}

OLedError setDisplayFullOn(uint32_t clock_pin, uint32_t data_pin)
{
    return sendCommand(clock_pin, data_pin, OLED_ALL_ON_DISPLAY);
}

OLedError setDisplayRAMMode(uint32_t clock_pin, uint32_t data_pin)
{
    return sendCommand(clock_pin, data_pin, OLED_RAM_DISPLAY);
}

OLedError turnOffDisplay(uint32_t clock_pin, uint32_t data_pin)
{
    return sendCommand(clock_pin, data_pin, OLED_TURN_OFF);
}

OLedError turnOnDisplay(uint32_t clock_pin, uint32_t data_pin)
{
    return sendCommand(clock_pin, data_pin, OLED_TURN_ON);
}

OLedError sendCommand(uint32_t clock_pin, uint32_t data_pin, uint8_t command)
{
    uint8_t err;
    startCondition(clock_pin, data_pin);

    err = setupCommand();
    if (err)
    {
        return err;
    }

    err = writeByteAndReadAck(command);
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

OLedError setupCommand()
{
    uint8_t err;

    // Slave address and ack bit
    err = writeByteAndReadAck(OLED_ADDR);
    if (err)
    {
        return err;
    }

    // Command byte
    err = writeByteAndReadAck(OLED_COMMAND_BYTE);
    if (err)
    {
        return err;
    }

    return OLED_I2C_OK;
}

OLedError setupData()
{
    uint8_t err;

    // Slave address and ack bit
    err = writeByteAndReadAck(OLED_ADDR);
    if (err)
    {
        return err;
    }

    // Command byte
    err = writeByteAndReadAck(OLED_DATA_BYTE);
    if (err)
    {
        return err;
    }

    return OLED_I2C_OK;
}

OLedError writeByteAndReadAck(uint8_t data)
{
    uint8_t buffer[1], err;

    err = write_i2c(&data, 8);
    if (err)
    {
        return err;
    }

    err = read_i2c(buffer, 1);

    if (!buffer[0] || err)
    {
        return OLED_I2C_ERROR;
    }

    return OLED_I2C_OK;
}
