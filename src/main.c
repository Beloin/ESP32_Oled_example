#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "debug.h"

// Separation of activities:
//  1. Define the snake animation
//      1.1 Animation will be defined as chars of 'U', 'D', 'F' and 'B'. Representing "UP", "DOWN", "FORWARDS", "BACKWARDS" respectively
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

// TODO: Add it inside Oled + SSD1306

#include "driver.h"

void app_main()
{
    debug("Starting...\n");
    MonitorResource *resource = Monitor.open();

    if (resource == NULL) {
        printf("Boo!\n");
        return;
    } else {
        printf("Resource %d\n", (int) resource);
    }

    debug("Starting main loop\n");
    while (1)
    {
        Monitor.print(resource);
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}