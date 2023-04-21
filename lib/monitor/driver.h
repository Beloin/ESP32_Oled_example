#ifndef ESP32_OLED_MONITOR_DRIVER_H
#define ESP32_OLED_MONITOR_DRIVER_H

#include <stdio.h>

// Following linux driver default interface OPEN, CLOSE, READ, WRITE, IOCTL

#define HEIGHT 32
#define WIDTH 128

typedef struct
{
    int const height;
    int const width;
    int const _index;
    uint8_t const *data;
} MonitorResource;

MonitorResource const *open();

void close(MonitorResource const *monitorResource);

void write(const MonitorResource *const matrix);

// void read(); // Is not implemented since we are using I^2C

/**
 * Blinks the Screen.
 * @param ms Blink time
 * @param monitor The monitor resource
 */
void ioctl_blink(const MonitorResource const *monitor, int ms);

#endif