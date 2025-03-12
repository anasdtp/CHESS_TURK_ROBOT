#include "GPIO.h"

void setupGPIO(){
    pinMode(STEP1, OUTPUT);
    pinMode(DIR1, OUTPUT);
    pinMode(STEP2, OUTPUT);
    pinMode(DIR2, OUTPUT);
    pinMode(STEP3, OUTPUT);
    pinMode(DIR3, OUTPUT);
    pinMode(STEP4, OUTPUT);
    pinMode(DIR4, OUTPUT);

    pinMode(FDC1, INPUT_PULLDOWN);
    pinMode(FDC2, INPUT_PULLDOWN);
    pinMode(FDC3, INPUT_PULLDOWN);

    pinMode(SERVO, OUTPUT);
    
    pinMode(STEPPER_MS1, OUTPUT);
    pinMode(STEPPER_MS2, OUTPUT);
    pinMode(STEPPER_MS3, OUTPUT);
}