#include <Tasks.h>
#include <stdio.h>
#include <driver/gpio.h>

#include <Defs.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// TaskHandle_t keyboardTaskHandle;

// bool keyboardButtonPressed = false;
bool keyboardButtonPressed;
int lastKeyboardButtonPressed;

void IRAM_ATTR keyboardIsr()
{
    uint16_t scanval = 0;
    for (int i = 0; i < 11; i++)
    {
        while (gpio_get_level(KEYBOARD_CLOCK_PIN))
            ;
        scanval |= gpio_get_level(KEYBOARD_DATA_PIN) << i;
        while (!gpio_get_level(KEYBOARD_CLOCK_PIN))
            ;
    }
    scanval >>= 1;
    scanval &= 0xFF;

    if (lastKeyboardButtonPressed != 240)
    {
        keyboardButtonPressed = true;
        lastKeyboardButtonPressed = scanval;
    }
    else
    {
        keyboardButtonPressed = false;
        lastKeyboardButtonPressed = 0;
    }
}

void keyboardTask(void *parameter)
{
    State *statePointer = (State *)parameter;
    while (1)
    {
    }
    // TODO : WHAT THE FUCK
    // vTaskDelete(keyboardTaskHandle);
}
