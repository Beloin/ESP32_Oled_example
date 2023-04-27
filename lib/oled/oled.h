#ifndef ESP32_OLED_OLED_H
#define ESP32_OLED_OLED_H

#include <stdlib.h>

#define OLED_SLAVE_ADDRESS 0b01111010

#define OLED_DATA_BYTE 0b01000000
#define OLED_COMMAND_BYTE 0b00000000

#define OLED_CONTRAST_CONTROL 0b10000001
#define OLED_ADDR 0b01111000

#define OLED_TURN_ON 0xAF
#define OLED_TURN_OFF 0xAE

#define OLED_RAM_DISPLAY 0xA4
#define OLED_ALL_ON_DISPLAY 0xA5

#define OLED_PAGE_0_ADDRESS 0xB0
#define OLED_MAX_PAGES 8

#define OLED_PAGE_ADDRESSING_MODE 0x22 // 0b0010_0010

// This setup lower (right) nibble for the 128 column addresses
#define OLED_LOWER_COLUMN_0_ADDRESS 0 
// This setup hihger (left) nibble for the 128 column addresses
#define OLED_HIGHER_COLUMN_0_ADDRESS 0x10

#define OLED_HEIGHT 32
#define OLED_WIDTH 128

typedef uint8_t OLedError;

const OLedError OLED_OUT_OF_BOUNDS = 12;

// OLedError setPixel(uint8_t x, uint8_t y, uint8_t v);
OLedError startDisplay(uint32_t clock_pin, uint32_t data_pin);

OLedError turnOffDisplay();

OLedError updateDisplay(uint8_t *data);

#endif