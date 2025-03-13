#ifndef GPIO_H
#define GPIO_H
#include <Arduino.h>

#define STEP1 GPIO_NUM_18
#define DIR1 GPIO_NUM_13
#define STEP2 GPIO_NUM_32
#define DIR2 GPIO_NUM_33
#define STEP3 GPIO_NUM_25
#define DIR3 GPIO_NUM_26
#define STEP4 GPIO_NUM_27
#define DIR4 GPIO_NUM_14

#define FDC1 GPIO_NUM_5
#define FDC2 GPIO_NUM_17
#define FDC3 GPIO_NUM_16

#define SERVO GPIO_NUM_4

#define STEPPER_MS1 GPIO_NUM_0
#define STEPPER_MS2 GPIO_NUM_2
#define STEPPER_MS3 GPIO_NUM_15

void setupGPIO();


#endif