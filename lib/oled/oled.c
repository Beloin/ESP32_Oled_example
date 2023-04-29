#include <stdio.h>
#include <oled.h>
#include <i2c.h>
#include "driver/gpio.h"

OLedError startDisplay(uint32_t clock_pin, uint32_t data_pin)
{
    uint8_t err = setup_i2c(clock_pin, data_pin);

    // Start condition
    gpio_set_direction(clock_pin, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(data_pin, GPIO_MODE_DEF_OUTPUT);

    // Data pin shound be used with a Pullup resistor
    gpio_set_level(data_pin, 1);
    gpio_set_level(clock_pin, 1);
    gpio_set_level(data_pin, 0);


    if (err)
    {
        return OLED_I2C_ERROR;
    }

    uint8_t buffer[1];

    // Slave address and ack bit
    write_i2c(OLED_ADDR, 8);
    read_i2c(buffer, 1);

    if (!buffer[0])
    {
        return OLED_I2C_ERROR;
    }

    // Command bytee
    write_i2c(OLED_COMMAND_BYTE, 8);
    read_i2c(buffer, 1);

    if (!buffer[0])
    {
        return OLED_I2C_ERROR;
    }

    // Turn monitor on
    write_i2c(OLED_TURN_ON, 8);
    read_i2c(buffer, 1);

    if (!buffer[0])
    {
        return OLED_I2C_ERROR;
    }
}
