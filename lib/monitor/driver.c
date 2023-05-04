#include "driver.h"
#include "oled.h"

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "debug.h"

#define CLOCK_PIN 13
#define DATA_PIN 4

MonitorResource *openMonitor();
void closeMonitor(MonitorResource *monitorResource);
void write(MonitorResource *matrix);
void ioctl_blink(MonitorResource *monitor, int ms);
void print_monitor(MonitorResource *monitorResource);
// void read(); // Is not implemented since we are using I^2C

MonitorResource *openMonitor()
{
    debug("Open Called...\n");

    // TODO: Continue from here
    uint8_t err;

    if ((err = startDisplay(CLOCK_PIN, DATA_PIN)))
    {
        return NULL;
    }

    debug("Display Started...\n");

    MonitorResource *mPointer = (MonitorResource *)malloc(sizeof(MonitorResource));
    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * OLED_HEIGHT * OLED_WIDTH);

    // MonitorResource toCopy = (MonitorResource){
    //     OLED_HEIGHT,
    //     OLED_WIDTH,
    //     0,
    //     data,
    // };

    mPointer->data = data;
    mPointer->height = OLED_HEIGHT;
    mPointer->width = OLED_WIDTH;
    mPointer->_index = 0;

    // memcpy(mPointer, &toCopy, sizeof(MonitorResource));

    return mPointer;
}

void closeMonitor(MonitorResource *monitorResource)
{
    turnOffDisplay(CLOCK_PIN, DATA_PIN);
    free(monitorResource->data);
    free(monitorResource);
}

void write(MonitorResource *matrix)
{
    updateDisplay(CLOCK_PIN, DATA_PIN, matrix->data);
}

void ioctl_blink(MonitorResource *monitor, int ms)
{
    setDisplayFullOn(CLOCK_PIN, DATA_PIN);

    const TickType_t tickMs = (ms) / portTICK_PERIOD_MS;
    vTaskDelay(tickMs);

    setDisplayRAMMode(CLOCK_PIN, DATA_PIN);
}

void print_monitor(MonitorResource *monitorResource)
{
    printf("{ \n");
    printf("  Height: %d\n", monitorResource->height);
    printf("  Width: %d\n", monitorResource->width);
    printf("}\n");
}

const struct monitor Monitor = {
    .open = openMonitor,
    .close = closeMonitor,
    .write = write,
    .ioctl_blink = ioctl_blink,
    .print = print_monitor,
};