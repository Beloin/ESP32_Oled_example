#include "driver.h"
#include "oled.h"

int index = 0;

MonitorResource const *open();
void close(MonitorResource const *monitorResource);
void write(const MonitorResource *const matrix);
void ioctl_blink(const MonitorResource const *monitor, int ms);
// void read(); // Is not implemented since we are using I^2C

MonitorResource const *open()
{
    MonitorResource *mPointer = (MonitorResource *)malloc(sizeof(MonitorResource));
    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * OLED_HEIGHT * OLED_WIDTH);

    MonitorResource toCopy = (MonitorResource){
        OLED_HEIGHT,
        OLED_WIDTH,
        index,
        data,
    };

    memcpy(mPointer, &toCopy, sizeof(MonitorResource));

    return mPointer;
}

void close(MonitorResource const *monitorResource)
{
    free(monitorResource->data);
    free(monitorResource);
}

// TODO: Add here OLED interface usage
void write(const MonitorResource *const matrix)
{
}

void ioctl_blink(const MonitorResource const *monitor, int ms)
{
    // TODO: Unecessary, since we have a native blink inside OLED
    int size = monitor->height * monitor->width;
    uint8_t blink_data[size];
    for (int i = 0; i < size; i++)
    {
        blink_data[i] = 1;
    }

    // ... send blik_data to OLED.
}

const struct monitor Monitor = {
    .open = open,
    .close = close,
    .write = write,
    .ioctl_blink = ioctl_blink,
};