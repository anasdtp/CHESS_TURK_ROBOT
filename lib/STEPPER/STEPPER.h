#ifndef STEPPER_H
#define STEPPER_H
#include <Arduino.h>    
#include <GPIO.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define DEG_PER_STEP 1.8f //Celui de base
#define STEPS_PER_REVOLTION 200 //Celui de base

#define METER_PER_REVOLTION_1 0.003175f //A mesurer pour chaque moteur.
#define METER_PER_REVOLTION_2 0.003175f //A mesurer pour chaque moteur.
#define METER_PER_REVOLTION_3 0.003175f //A mesurer pour chaque moteur. 
//Faire tourner le moteur d'un tour et mesurer la distance parcourue en metres pour chaque axe

#define NB_STEPPER 3

struct Position
{
    float x{}, y{}, z{};
};

class STEPPER
{
public:
    AccelStepper *stepper1;//(AccelStepper::FULL2WIRE, STEP2, DIR2);
    AccelStepper *stepper2;//(AccelStepper::FULL2WIRE, STEP3, DIR3);
    AccelStepper *stepper3;//(AccelStepper::FULL2WIRE, STEP4, DIR4);

    STEPPER(MultiStepper *steppers);

    void configureMicrostepping(bool ms3, bool ms2, bool ms1);

    /**
     * @brief Move the stepper to the specified position
     * @param x The x position in meters
     * @param y The y position in meters
     * @param z The z position in meters
     */
    void moveTo(Position pos);

    void homing();

private:
    MultiStepper *steppers;

    bool ms1, ms2, ms3;

    float deg_per_step;
    float rad_per_step;
    float step_per_meter[NB_STEPPER];

    long positions[NB_STEPPER]; // Array of desired stepper positions

};




#endif // STEPPER_H