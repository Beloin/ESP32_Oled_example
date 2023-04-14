#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

// Using in-board led
#define LED 2

char status[] = {'L', 'H'};

void app_main()
{
    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_DEF_OUTPUT);

    printf("Hello World!\n");

    bool i = 0;
    for (;;)
    {
        i = i ^ 1;
        gpio_set_level(LED, i);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("Delayed... %c\n", status[!i]);
        fflush(stdout);
    }
}