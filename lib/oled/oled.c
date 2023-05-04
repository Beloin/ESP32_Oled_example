#include <stdio.h>
#include <oled.h>
#include <i2c.h>
#include "driver/gpio.h"
#include <debug.h>

const OLedStatus OLED_OUT_OF_BOUNDS = 12;
const OLedStatus OLED_I2C_ERROR = 13;
const OLedStatus OLED_I2C_OK = 0;

void startCondition(uint32_t clock_pin, uint32_t data_pin);
void endCondition(uint32_t clock_pin, uint32_t data_pin);

OLedStatus sendCommand(uint32_t clock_pin, uint32_t data_pin, uint8_t command);
OLedStatus writeByteAndReadAck(uint8_t data);
OLedStatus setupCommand();
OLedStatus setupData();

OLedStatus startDisplay(uint32_t clock_pin, uint32_t data_pin)
{
    debug("Starting Display...\n");
    uint8_t err = setup_i2c(clock_pin, data_pin);
    if (err)
    {
        return OLED_I2C_ERROR;
    }

    return sendCommand(clock_pin, data_pin, OLED_TURN_ON);
}

OLedStatus updateDisplay(uint32_t clock_pin, uint32_t data_pin, uint8_t *data) // Data is OLED_HEIGHT * OLED_WIDTH
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

OLedStatus setDisplayFullOn(uint32_t clock_pin, uint32_t data_pin)
{
    return sendCommand(clock_pin, data_pin, OLED_ALL_ON_DISPLAY);
}

OLedStatus setDisplayRAMMode(uint32_t clock_pin, uint32_t data_pin)
{
    return sendCommand(clock_pin, data_pin, OLED_RAM_DISPLAY);
}

OLedStatus turnOffDisplay(uint32_t clock_pin, uint32_t data_pin)
{
    return sendCommand(clock_pin, data_pin, OLED_TURN_OFF);
}

OLedStatus turnOnDisplay(uint32_t clock_pin, uint32_t data_pin)
{
    return sendCommand(clock_pin, data_pin, OLED_TURN_ON);
}

OLedStatus sendCommand(uint32_t clock_pin, uint32_t data_pin, uint8_t command)
{
    debug("Sending Command to OLED\n");
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
    // TODO: Start condition shoul wait too
    
    // Data pin shound be used with a Pullup resistor
    gpio_set_direction(clock_pin, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(data_pin, GPIO_MODE_DEF_OUTPUT);

    gpio_set_level(data_pin, 1);
    i2c_timer();
    gpio_set_level(clock_pin, 1);
    i2c_timer();
    gpio_set_level(data_pin, 0);
    i2c_timer();
}

void endCondition(uint32_t clock_pin, uint32_t data_pin)
{
    // TODO: end condition shoul wait too

    gpio_set_direction(clock_pin, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(data_pin, GPIO_MODE_DEF_OUTPUT);

    // Data pin shound be used with a Pullup resistor
    gpio_set_level(data_pin, 0);
    i2c_timer();
    gpio_set_level(clock_pin, 1);
    i2c_timer();
    gpio_set_level(data_pin, 1);
    i2c_timer();
}

OLedStatus setupCommand()
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

OLedStatus setupData()
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

OLedStatus writeByteAndReadAck(uint8_t data)
{
    uint8_t buffer[1], err;

    err = write_i2c(&data, 8);
    if (err)
    {
        return err;
    }

    err = read_i2c(buffer, 1);

    // TODO: Re-enable this
    // if (!buffer[0] || err)
    // {
    //     return OLED_I2C_ERROR;
    // }

    return OLED_I2C_OK;
}
