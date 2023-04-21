#include "snake.h"
#include "driver.h"

void upwards(Snake *const snake, MonitorResource const *resource);
void downwards(Snake *const snake, MonitorResource const *resource);
void fowards(Snake *const snake, MonitorResource const *resource);
void backwards(Snake *const snake, MonitorResource const *resource);

// TODO: First of we will use one pixel.

void DrawAnimation(Snake *const snake, char *animation, int animation_size)
{
    MonitorResource const *mnResource = Monitor.open();

    for (int i = 0; i < animation_size; i++)
    {
        switch (animation[i])
        {
        case 'U':
            upwards(snake, mnResource);
            break;
        case 'D':
            downwards(snake, mnResource);
            break;
        case 'F':
            fowards(snake, mnResource);
            break;
        case 'B':
            backwards(snake, mnResource);
            break;
        default:
            printf("Char not Available.\n");
            break;
        }
    }

    Monitor.close(mnResource);
}

void upwards(Snake *const snake, MonitorResource const *resource)
{
    int newY = snake->y + ANIMATION_STEP;
    int division = newY / resource->height;
    int rest = resource->height % newY;

    snake->y = division * rest;
}

void downwards(Snake *const snake, MonitorResource const *resource)
{
    int newY = snake->y - ANIMATION_STEP;
    int division = newY / resource->height;
    int rest = resource->height % newY;

    snake->y = division * rest;
}

void fowards(Snake *const snake, MonitorResource const *resource)
{
    int newX = snake->x + ANIMATION_STEP;
    int division = newX / resource->width;
    int rest = resource->width % newX;

    snake->x = division * rest;
}

void backwards(Snake *const snake, MonitorResource const *resource)
{
    int newX = snake->x - ANIMATION_STEP;
    int division = newX / resource->width;
    int rest = resource->width % newX;

    snake->x = division * rest;
}