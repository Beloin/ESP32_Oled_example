
#ifndef ESP32_OLED_SNAKE_H
#define ESP32_OLED_SNAKE_H

#include <stdio.h>

#define SNAKE_SIZE 5
#define MAX_ANIMATION_LENGTH 10
#define ANIMATION_STEP 2

typedef struct
{
    int x, y;
    uint8_t lenght;
} Snake;

uint8_t const animation_01[MAX_ANIMATION_LENGTH] =
    {'F', 'F', 'D', 'D', 'B', 'U', 'U', 'D', 'F', 'F'};

void DrawAnimation01(Snake *const snake, char *animation_01, int animation_size);

#endif