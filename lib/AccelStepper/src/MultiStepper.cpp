// MultiStepper.cpp
//
// Copyright (C) 2015 Mike McCauley
// $Id: MultiStepper.cpp,v 1.3 2020/04/20 00:15:03 mikem Exp mikem $

#include "MultiStepper.h"
#include "AccelStepper.h"

MultiStepper::MultiStepper() : _num_steppers(0)
{
}

boolean MultiStepper::addStepper(AccelStepper *stepper)
{
	if (_num_steppers >= MULTISTEPPER_MAX_STEPPERS)
	{
		return false;
	} // No room for more
	_steppers[_num_steppers++] = stepper;
	return true;
}

void MultiStepper::moveTo(long absolute[])
{
    // First find the stepper that will take the longest time to move
    float longestTime = 0.0;

    uint8_t i;
    for (i = 0; i < _num_steppers; i++)
    {
        long thisDistance = absolute[i] - _steppers[i]->currentPosition();
        float thisTime = abs(thisDistance) / _steppers[i]->maxSpeed();

        if (thisTime > longestTime)
        {
            longestTime = thisTime;
        }
    }

    if (longestTime > 0.0)
    {
        // Now calculate the required acceleration and speed for each stepper
        for (i = 0; i < _num_steppers; i++)
        {
            long thisDistance = absolute[i] - _steppers[i]->currentPosition();
            float thisSpeed = abs(thisDistance) / longestTime;

            // Set the target position
            _steppers[i]->moveTo(absolute[i]);

            // Adjust the max speed and acceleration to synchronize arrival
            _steppers[i]->setMaxSpeed(thisSpeed);
            _steppers[i]->setAcceleration(_steppers[i]->acceleration()); // Use the stepper's max acceleration
        }
    }
    
}

// Returns true if any motor is still running to the target position.
boolean MultiStepper::run()
{
    uint8_t i;
    boolean ret = false;
    for (i = 0; i < _num_steppers; i++)
    {
        if (_steppers[i]->distanceToGo() != 0)
        {
            _steppers[i]->run(); // Use run() to handle acceleration and deceleration
            ret = true;
        }
    }
    return ret;
}

// Blocks until all steppers reach their target position and are stopped
void MultiStepper::runSpeedToPosition()
{
	while (run())
		;
}
