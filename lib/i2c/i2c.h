#ifndef ESP32_OLED_I2C_H
#define ESP32_OLED_I2C_H

#include <stdio.h>

// Communication defined in Khz
#define I2C_SPEED 100

#define I2C_NOT_INITIALIZED 1
#define I2C_NO_HEAP_SPACE 2
#define I2C_ALREADY_INITIALIZED 3
#define I2C_OK 0

// Starts by stabilishing connection.
// Then send commands and data

/**
 * Should be called once for application.
*/
uint8_t setup_i2c(uint32_t clockPin, uint32_t dataPin);

uint8_t write_i2c(uint8_t *data, int bit_lenght);

uint8_t read_i2c(uint8_t *data, int bit_lenght);

uint8_t close_i2c();

#endif