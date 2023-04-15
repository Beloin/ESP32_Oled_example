#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"


// Separation of activities:
//  1. Define the snake animation
//  2. Define Snake Sprite
//  3. Define place of animations -> For now we can get from code, but later we will get from EESP32's FLASH EEPROM
//  4. Define how to get Snake Sprite -> For now we can get from code, but later we will get from EESP32's FLASH EEPROM
//  5. Simulate animation
//      5.1 Remember to simulate in real time, waiting for updates in Screen
//      5.2 (Optional) Show in linux terminal or file
//  6. Create OLED (I2C) driver interface (Read datasheet for SSD1306)
//      6.1 Driver Interface
//      6.2 Graphic Interface: In order to hide how inside the driver works, we will only use a 128x32 or 128x64 Matrix.
//  7. Show simulation on OLED
//
//  Not Required:
//  8. Load Snake sprite from Flash
//  9. Make animations "templates"
//  10. Load animation templates from Flash


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