#include <stdio.h>

#include <Defs.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifndef TASKS_H
#define TASKS_H

// TaskHandle_t audioTaskHandle;
void audioTask(void *parameter);

// // bool keyboardButtonPressed = false;
// bool keyboardButtonPressed;
// int lastKeyboardButtonPressed;
void IRAM_ATTR keyboardIsr();
// TaskHandle_t keyboardTaskHandle;
void keyboardTask(void *parameter);

// TaskHandle_t sequencerTaskHandle;
void sequencerTask(void *parameter);

#endif // TASKS_H