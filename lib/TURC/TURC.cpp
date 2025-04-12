#include "TURC.h"

TURC::TURC(MultiStepper *steppers, Servo *servo) : CommunicationPC() 
{
    this->steppers = steppers;
    this->servo = servo;
    setupGPIO();
    configureMicrostepping(false, false, false);

    this->stepper1 = new AccelStepper(AccelStepper::FULL2WIRE, STEP1, DIR1);//x
    this->stepper2 = new AccelStepper(AccelStepper::FULL2WIRE, STEP2, DIR2);//y
    this->stepper3 = new AccelStepper(AccelStepper::FULL2WIRE, STEP3, DIR3);//z
    // this->stepper4 = new AccelStepper(AccelStepper::FULL2WIRE, STEP4, DIR4);//On a enlever un moteur

    this->stepper1->setMaxSpeed(1000);
    this->stepper2->setMaxSpeed(1000);
    this->stepper3->setMaxSpeed(1000);
    // this->stepper4->setMaxSpeed(1000);

    this->stepper1->setAcceleration(1000);
    this->stepper2->setAcceleration(1000);
    this->stepper3->setAcceleration(1000);
    // this->stepper4->setAcceleration(1000);

    this->steppers->addStepper(this->stepper1);
    this->steppers->addStepper(this->stepper2);
    this->steppers->addStepper(this->stepper3);
    // this->steppers->addStepper(this->stepper4);

    Position pos_min = {0, 0, 0};
    Position pos_max = {0.4, 0.4, 0.2};
    setPosMin_Max(pos_min, pos_max);

    current = new Position();
    current->x = 0;
    current->y = 0;
    current->z = 0;

    this->attachPosition(current);

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
        Serial.println("Position out of bounds");
        return;
    }
    positions[0] = pos.x * this->step_per_meter[0];//x
    positions[1] = pos.y * this->step_per_meter[1];//y
    positions[2] = pos.z * this->step_per_meter[2];//z
    
    this->steppers->moveTo(positions);
    // this->steppers->runSpeedToPosition(); // Blocks until all are in position
}

void TURC::moveTo(Position *pos){
    if(pos == nullptr){
        Serial.println("Position is null");
        return;
    }
    if(*pos>pos_max || *pos<pos_min){
        return;
    }
    positions[0] = pos->x * this->step_per_meter[0];//x
    positions[1] = pos->y * this->step_per_meter[1];//y
    positions[2] = pos->z * this->step_per_meter[2];//z
    
    this->steppers->moveTo(positions);
    // this->steppers->runSpeedToPosition(); // Blocks until all are in position
}

Position *TURC::getCurrentPosition(){
    current->x = this->stepper1->currentPosition() / this->step_per_meter[0];
    current->y = this->stepper2->currentPosition() / this->step_per_meter[2];
    current->z = this->stepper3->currentPosition() / this->step_per_meter[3];
    return current;
}

bool TURC::homing() {
    static int axe = 0;

    switch (axe) {
        case 0:
            this->stepper1->moveTo(-10000000);
            this->stepper2->moveTo(-10000000);
            this->stepper3->moveTo(-10000000);
            axe = 1;
            break;

        case 1://X
            if (FDC1Pressed == LOW) {
                this->stepper1->run();
            } else {
                this->stepper1->stop();
                this->stepper1->setCurrentPosition(0);
                axe = 2;
            }
            break;

        case 2://Y
            if (FDC2Pressed == LOW) {
                this->stepper2->run();
            } else {
                this->stepper2->stop();
                this->stepper2->setCurrentPosition(0);
                axe = 3;
            }
            break;

        case 3://Z
            if (FDC3Pressed == LOW) {
                this->stepper3->run();
            } else {
                this->stepper3->stop();
                this->stepper3->setCurrentPosition(0);
                axe = 0; // Reset axe to indicate homing is complete
                return true; // Homing is complete
            }
            break;

        default:
            axe = 0;
            break;
    }

    return false;
}

void TURC::setPosMin_Max(Position pos_min, Position pos_max){
    this->pos_min = pos_min;
    this->pos_max = pos_max;
}

void TURC::machine(){
    static unsigned long last_time = 0;
    switch (state)
    {
    case WAIT:
    {
        if(this->newMoveReceived()){
            MOVE *move = this->getMove();
            if(move == nullptr){
                return;
            }
            if(move->type == XYT_MOVE){
                this->moveTo(&move->pos);
                state = RUN;
            }
            else if(move->type == SERVO_GRAB_MOVE){
                state = SERVO_GRAB;
            }
            else if(move->type == SERVO_RELEASE_MOVE){
                state = SERVO_RELEASE;
            }
            else if(move->type == HOMING_MOVE){
                state = HOMING;
            }    
        }
        // if(millis() - last_time > 1000){
        //     last_time = millis();
        //     sendMsg(ID_ACK_SERVO_GRAB);
        // }
    }
        break;
    case RUN:
    {
        // if(FDC1Pressed){
        //     this->stepper1->stop();
        //     this->stepper1->setCurrentPosition(0);
        // }
        // if(FDC2Pressed){
        //     this->stepper2->stop();
        //     this->stepper2->setCurrentPosition(0);
        // }   
        // if(FDC3Pressed){
        //     this->stepper3->stop();
        //     this->stepper3->setCurrentPosition(0);
        // }
        if(!this->steppers->run()){
            this->getCurrentPosition();
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
    case HOMING:
    {
        if(homing()){
            state = WAIT;
        }
    }
        break;
    default:
        state = WAIT;
        break;
    }
}