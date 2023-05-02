#ifndef ESP32_OLED_MONITOR_DRIVER_H
#define ESP32_OLED_MONITOR_DRIVER_H

#include <stdio.h>

typedef struct
{
    int height;
    int width;
    int _index;
    uint8_t *data; // TODO: Later try to do it with a 4096 bits instead of 32768
} MonitorResource;

// Following linux driver default interface OPEN, CLOSE, READ, WRITE, IOCTL;
struct monitor
{
    MonitorResource *(*open)(void);
    void (*close)(MonitorResource *);
    void (*write)(MonitorResource *);
    /**
     * Blinks the Screen.
     * @param ms Blink time
     * @param monitor The monitor resource
     */
    void (*ioctl_blink)(MonitorResource *, int);
    void (*print)(MonitorResource *);
};

/**
 * Monitor Namespace to handle MonitorResource
 */
extern const struct monitor Monitor;

#endif