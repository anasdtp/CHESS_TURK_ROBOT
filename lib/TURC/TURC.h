#ifndef STEPPER_H
#define STEPPER_H
#include <Arduino.h>    
#include <Commun.h>
#include <CommunicationPC.h>
#include <GPIO.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <ESP32Servo.h>

#define DEG_PER_STEP 1.8f //Celui de base
#define STEPS_PER_REVOLTION 200 //Celui de base

#define METER_PER_REVOLTION_1 0.015f //A mesurer pour chaque moteur.
#define METER_PER_REVOLTION_2 0.015f //A mesurer pour chaque moteur.
#define METER_PER_REVOLTION_3 0.015f //A mesurer pour chaque moteur. 
#define METER_PER_REVOLTION_4 0.006f //A mesurer pour chaque moteur. 
//Faire tourner le moteur d'un tour et mesurer la distance parcourue en metres pour chaque axe

#define NB_STEPPER 4

class TURC : public CommunicationPC
{
public:
    AccelStepper *stepper1;//(AccelStepper::FULL2WIRE, STEP1, DIR1);
    AccelStepper *stepper2;//(AccelStepper::FULL2WIRE, STEP2, DIR2);
    AccelStepper *stepper3;//(AccelStepper::FULL2WIRE, STEP3, DIR3);
    // AccelStepper *stepper4;//(AccelStepper::FULL2WIRE, STEP4, DIR4);

    float deg_per_step;

    TURC(MultiStepper *steppers, Servo *servo);

    void configureMicrostepping(bool ms3, bool ms2, bool ms1);

    /**
     * @brief Move the stepper to the specified position
     * @param x The x position in meters
     * @param y The y position in meters
     * @param z The z position in meters
     */
    void moveTo(Position pos);
    void moveTo(Position *pos);

    Position *getCurrentPosition();

    bool homing();

    void machine();

    void setPosMin_Max(Position pos_min, Position pos_max);

private:
    MultiStepper *steppers;
    Servo *servo;

    bool ms1, ms2, ms3;

    
    float rad_per_step;
    float step_per_meter[NB_STEPPER];

    long positions[NB_STEPPER]; // Array of desired stepper positions

    Position pos_max;
    Position pos_min;
    Position *current;

    // État de la réception
    enum StateStepper{
        WAIT,
        RUN,
        SERVO_GRAB,
        SERVO_RELEASE,
        HOMING,
      };

    StateStepper state;

};




#endif // STEPPER_H