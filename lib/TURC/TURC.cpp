#include "TURC.h"

TURC::TURC(MultiStepper *steppers, Servo *servo) : CommunicationPC() 
{
    this->steppers = steppers;
    this->servo = servo;
    setupGPIO();
    configureMicrostepping(false, false, false);

    this->stepper1 = new AccelStepper(AccelStepper::FULL2WIRE, STEP1, DIR1);
    this->stepper2 = new AccelStepper(AccelStepper::FULL2WIRE, STEP2, DIR2);
    this->stepper3 = new AccelStepper(AccelStepper::FULL2WIRE, STEP3, DIR3);
    this->stepper4 = new AccelStepper(AccelStepper::FULL2WIRE, STEP4, DIR4);

    this->stepper1->setMaxSpeed(4000);
    this->stepper2->setMaxSpeed(4000);
    this->stepper3->setMaxSpeed(4000);
    this->stepper4->setMaxSpeed(4000);

    this->stepper1->setAcceleration(1000);
    this->stepper2->setAcceleration(1000);
    this->stepper3->setAcceleration(1000);
    this->stepper4->setAcceleration(1000);

    this->steppers->addStepper(*this->stepper1);
    this->steppers->addStepper(*this->stepper2);
    this->steppers->addStepper(*this->stepper3);
    this->steppers->addStepper(*this->stepper4);

    Position pos_min = {0, 0, 0};
    Position pos_max = {0.6, 0.6, 0.6};
    setPosMin_Max(pos_min, pos_max);

    state = WAIT;
}

void TURC::configureMicrostepping(bool ms3, bool ms2, bool ms1){
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

void TURC::moveTo(Position pos){
    if(pos>pos_max || pos<pos_min){
        return;
    }
    positions[0] = pos.x * this->step_per_meter[0];
    positions[1] = pos.x * this->step_per_meter[1];
    positions[2] = pos.y * this->step_per_meter[2];
    
    this->steppers->moveTo(positions);
    // this->steppers->runSpeedToPosition(); // Blocks until all are in position
}

void TURC::moveTo(Position *pos){
    if(*pos>pos_max || *pos<pos_min){
        return;
    }
    positions[0] = pos->x * this->step_per_meter[0];
    positions[1] = pos->x * this->step_per_meter[1];
    positions[2] = pos->y * this->step_per_meter[2];
    
    this->steppers->moveTo(positions);
    // this->steppers->runSpeedToPosition(); // Blocks until all are in position
}

void TURC::homing(){
    this->stepper1->moveTo(-10000000);
    this->stepper2->moveTo(-10000000);
    while(FDC1Pressed == LOW){
        this->stepper1->run();
    }
    this->stepper1->stop();
    this->stepper2->stop();
    this->stepper1->setCurrentPosition(0);
    this->stepper2->setCurrentPosition(0);

    while(FDC2Pressed == LOW){
        this->stepper3->run();
    }
    this->stepper3->stop();
    this->stepper3->setCurrentPosition(0);
    
    this->stepper4->moveTo(-10000000);
    while(FDC3Pressed == LOW){
        this->stepper4->run();
    }
    this->stepper4->stop();
    this->stepper4->setCurrentPosition(0); 
}

void TURC::setPosMin_Max(Position pos_min, Position pos_max){
    this->pos_min = pos_min;
    this->pos_max = pos_max;
}

void TURC::machine(){
    switch (state)
    {
    case WAIT:
    {
        if(this->newMoveReceived()){
            this->moveTo(this->getMove());
            state = RUN;
        }
        else if(this->newServoGrabReceived()){
            state = SERVO_GRAB;
        }
        else if(this->newServoReleaseReceived()){
            state = SERVO_RELEASE;
        }
    }
        break;
    case RUN:
    {
        if(FDC1Pressed){
            this->stepper1->stop();
            this->stepper1->setCurrentPosition(0);
        }
        if(FDC2Pressed){
            this->stepper2->stop();
            this->stepper2->setCurrentPosition(0);
        }   
        if(FDC3Pressed){
            this->stepper3->stop();
            this->stepper3->setCurrentPosition(0);
        }
        if(!this->steppers->run()){
            state = WAIT;
        }
    }
        break;
    case SERVO_GRAB:
    {
        servo->write(180);
        delay(100);
        state = WAIT;
    }
        break;
    case SERVO_RELEASE:
    {
        servo->write(0);
        delay(100);
        state = WAIT;
    }
        break;
    default:
        state = WAIT;
        break;
    }
}