#ifndef ESP32_OLED_I2C_H
#define ESP32_OLED_I2C_H

#include <stdio.h>

#define CONTRAST_CONTROL 0b10000001

// Communication defined in Khz
#define SPEED 1


void send_i2c(uint8_t address);

#endif