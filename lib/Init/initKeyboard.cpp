#include <Init.h>
#include <stdio.h>
#include <driver/gpio.h>

#include <Defs.h>

// #include <keyboardTask.h>

bool initKeyboard()
{
    // // pinMode(KEYBOARD_CLOCK_PIN, INPUT);
    // // pinMode(KEYBOARD_DATA_PIN, INPUT);
    // // attachInterrupt(KEYBOARD_CLOCK_PIN, keyboardIsr, FALLING);

    // esp_rom_gpio_pad_select_gpio(KEYBOARD_CLOCK_PIN);
    // gpio_set_direction(KEYBOARD_CLOCK_PIN, GPIO_MODE_INPUT);
    // esp_rom_gpio_pad_select_gpio(KEYBOARD_DATA_PIN);
    // gpio_set_direction(KEYBOARD_DATA_PIN, GPIO_MODE_INPUT);

    // gpio_set_intr_type(KEYBOARD_CLOCK_PIN, GPIO_INTR_NEGEDGE);
    // // gpio_set_intr_type(KEYBOARD_CLOCK_PIN, GPIO_INTR_LOW_LEVEL);

    // gpio_intr_enable(KEYBOARD_CLOCK_PIN);
    // esp_intr_alloc(ETS_GPIO_INTR_SOURCE, 0, gpioHandler, "Test", NULL);

    // attachInterrupt(KEYBOARD_CLOCK_PIN, keyboardIsr, FALLING);

    return true;
}
