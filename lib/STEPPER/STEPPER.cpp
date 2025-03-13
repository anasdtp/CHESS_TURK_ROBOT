#include "STEPPER.h"

STEPPER::STEPPER(MultiStepper *steppers)
{
    this->steppers = steppers;
    setupGPIO();
    configureMicrostepping(false, false, false);

    this->stepper1 = new AccelStepper(AccelStepper::FULL2WIRE, STEP1, DIR1);
    this->stepper2 = new AccelStepper(AccelStepper::FULL2WIRE, STEP2, DIR2);
    this->stepper3 = new AccelStepper(AccelStepper::FULL2WIRE, STEP3, DIR3);

    this->stepper1->setMaxSpeed(4000);
    this->stepper2->setMaxSpeed(4000);
    this->stepper3->setMaxSpeed(4000);

    this->stepper1->setAcceleration(1000);
    this->stepper2->setAcceleration(1000);
    this->stepper3->setAcceleration(1000);

    this->steppers->addStepper(*this->stepper1);
    this->steppers->addStepper(*this->stepper2);
    this->steppers->addStepper(*this->stepper3);
}

void STEPPER::configureMicrostepping(bool ms3, bool ms2, bool ms1){
    this->ms1 = ms1;
    this->ms2 = ms2;
    this->ms3 = ms3;
    digitalWrite(STEPPER_MS1, this->ms1);
    digitalWrite(STEPPER_MS2, this->ms2);
    digitalWrite(STEPPER_MS3, this->ms3);

    this->deg_per_step = DEG_PER_STEP / (this->ms1 + 1) / (this->ms2 + 1) / (this->ms3 + 1);
    this->rad_per_step = this->deg_per_step * PI / 180.f;

    this->step_per_meter[0] = (360.f/this->deg_per_step) / METER_PER_REVOLTION_1;
    this->step_per_meter[1] = (360.f/this->deg_per_step) / METER_PER_REVOLTION_2;
    this->step_per_meter[2] = (360.f/this->deg_per_step) / METER_PER_REVOLTION_3;
}

void STEPPER::moveTo(Position pos){
    positions[0] = pos.x * this->step_per_meter[0];
    positions[1] = pos.y * this->step_per_meter[1];
    positions[2] = pos.z * this->step_per_meter[2];
    this->steppers->moveTo(positions);
    this->steppers->runSpeedToPosition(); // Blocks until all are in position
}

void STEPPER::homing(){
    this->stepper1->moveTo(-10000000);
    while(digitalRead(FDC1) == LOW){
        this->stepper1->run();
    }
    this->stepper1->stop();
    this->stepper1->setCurrentPosition(0);

    this->stepper1->moveTo(-10000000);
    while(digitalRead(FDC2)  == LOW){
        this->stepper2->run();
    }
    this->stepper2->stop();
    this->stepper2->setCurrentPosition(0);
    
    this->stepper1->moveTo(-10000000);
    while(digitalRead(FDC3) == LOW){
        this->stepper3->run();
    }
    this->stepper3->stop();
    this->stepper3->setCurrentPosition(0); 
}