#include "driver.h"
#include "oled.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define CLOCK_PIN 13
#define DATA_PIN 4

MonitorResource *open();
void close(MonitorResource *monitorResource);
void write(MonitorResource *matrix);
void ioctl_blink(MonitorResource *monitor, int ms);
// void read(); // Is not implemented since we are using I^2C

MonitorResource *open()
{
    uint8_t err;
    if ((err = startDisplay(CLOCK_PIN, DATA_PIN)))
    {
        return NULL;
    }

    MonitorResource *mPointer = (MonitorResource *)malloc(sizeof(MonitorResource));
    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * OLED_HEIGHT * OLED_WIDTH);

    MonitorResource toCopy = (MonitorResource){
        OLED_HEIGHT,
        OLED_WIDTH,
        0,
        data,
    };

    memcpy(mPointer, &toCopy, sizeof(MonitorResource));

    return mPointer;
}

void close(MonitorResource *monitorResource)
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

const struct monitor Monitor = {
    .open = open,
    .close = close,
    .write = write,
    .ioctl_blink = ioctl_blink,
};