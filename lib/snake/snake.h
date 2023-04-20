
#ifndef ESP32_OLED_SNAKE_H
#define ESP32_OLED_SNAKE_H

#include <stdio.h>

#define SNAKE_SIZE 5
#define MAX_ANIMATION_LENGTH 10

uint8_t const default_snake[SNAKE_SIZE];
uint8_t const animation_01[MAX_ANIMATION_LENGTH] =
    {'F', 'F', 'D', 'D', 'B', 'U', 'U', 'D', 'F', 'F'};

void DrawAnimation01(const uint8_t  *const snake);

#endif