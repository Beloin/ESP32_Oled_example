#include <stdio.h>
#include "i2c.h"
#include <freertos/semphr.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

SemaphoreHandle_t xSemaphore;
// TODO: This should be a struct to make i2c multi-use
uint32_t i2c_clockPin;
uint32_t i2c_dataPin;
uint8_t i2c_initialized = 0;

uint8_t setup_i2c(uint32_t clockPin, uint32_t dataPin)
{
    if (i2c_initialized)
    {
        return I2C_ALREADY_INITIALIZED;
    }

    xSemaphore = xSemaphoreCreateBinary();
    if (xSemaphore == NULL)
    {
        return I2C_NO_HEAP_SPACE;
    }

    // TODO: See if is necessary give first as its seen in the documentation.
    xSemaphoreGive(xSemaphore);

    i2c_clockPin = clockPin;
    i2c_dataPin = dataPin;

    esp_rom_gpio_pad_select_gpio(i2c_clockPin);
    esp_rom_gpio_pad_select_gpio(i2c_dataPin);

    i2c_initialized = 1;
}

uint8_t write_i2c(uint8_t address, uint8_t *data, int bit_lenght)
{
    if (!i2c_initialized)
    {
        return I2C_NOT_INITIALIZED;
    }

    while (!xSemaphoreTake(xSemaphore, portMAX_DELAY))
        ;

    gpio_set_direction(i2c_clockPin, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(i2c_dataPin, GPIO_MODE_DEF_OUTPUT);

    const TickType_t tickMs = (1 / I2C_SPEED) / portTICK_PERIOD_MS;

    int full_bit_size = 0;
    int i = 0;
    for (;;)
    {
        uint8_t value = data[i];

        // Run bits
        for (int j = 0; i < 8; j++)
        {
            gpio_set_level(i2c_clockPin, 0);

            uint8_t pin_value = (value >> j) & 0x1; // Masks all other bits but least significant bit
            gpio_set_level(i2c_dataPin, pin_value);
            full_bit_size++;

            gpio_set_level(i2c_clockPin, 1);

            // TODO: Add delay to respect i2c speed
            vTaskDelay(tickMs);

            if (full_bit_size >= bit_lenght)
            {
                break;
            }
        }

        if (full_bit_size >= bit_lenght)
        {
            break;
        }

        i++;
    }

    // Semaphore is obligatory to give.
    while (!xSemaphoreGive(xSemaphore))
        ;

    return I2C_OK;
}

uint8_t read_i2c(uint8_t address, uint8_t *data, int bit_lenght)
{
    if (!i2c_initialized)
    {
        return I2C_NOT_INITIALIZED;
    }

    while (!xSemaphoreTake(xSemaphore, portMAX_DELAY))
        ;

    gpio_set_direction(i2c_clockPin, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(i2c_dataPin, GPIO_MODE_DEF_INPUT);

    const TickType_t tickMs = (1 / I2C_SPEED) / portTICK_PERIOD_MS;

    // TODO: There's a better way to do this... run thorugn bits and use (i/8)*(i%8) to get
    int full_bit_size = 0;
    int i = 0;
    for (;;)
    {
        uint8_t *value = &data[i];
        *value = 0;

        // Run bits
        for (int j = 0; i < 8; j++)
        {
            gpio_set_level(i2c_clockPin, 0);
            gpio_set_level(i2c_clockPin, 1);

            uint8_t level = gpio_get_level(i2c_dataPin);
            full_bit_size++;

            // TODO: This works??
            *value = ((level << j) & 0xFF) & *value;

            gpio_set_level(i2c_clockPin, 0);

            // TODO: Add delay to respect i2c speed
            vTaskDelay(tickMs);

            if (full_bit_size >= bit_lenght)
            {
                break;
            }
        }

        if (full_bit_size >= bit_lenght)
        {
            break;
        }

        i++;
    }

    // Semaphore is obligatory to give.
    while (!xSemaphoreGive(xSemaphore))
        ;

    return I2C_OK;
}

uint8_t close_i2c()
{
    while (!xSemaphoreTake(xSemaphore, portMAX_DELAY))
        ;

    i2c_clockPin = 0;
    i2c_initialized = 0;
    i2c_dataPin = 0;

    vSemaphoreDelete(xSemaphore);

    return I2C_OK;
}