#ifndef ESP32_OLED_MONITOR_DRIVER_H
#define ESP32_OLED_MONITOR_DRIVER_H

#include <stdio.h>

// TODO: Put this into other area
#define HEIGHT 32
#define WIDTH 128

typedef struct
{
    int const height;
    int const width;
    int const _index;
    uint8_t const *data; // TODO: Later try to do it with a 4096 bits instead of 32768
} MonitorResource;

// Following linux driver default interface OPEN, CLOSE, READ, WRITE, IOCTL;
struct monitor
{
    MonitorResource const *(*open)(void);
    void (*close)(MonitorResource const *);
    void (*write)(const MonitorResource *const);
    /**
     * Blinks the Screen.
     * @param ms Blink time
     * @param monitor The monitor resource
     */
    void (*ioctl_blink)(const MonitorResource const *, int);
};

/**
 * Monitor Namespace to handle MonitorResource
 */
extern const struct monitor Monitor;

#endif