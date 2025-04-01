#include <Arduino.h>
#include <GPIO.h>
#include <TURC.h>

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper *steppers;
TURC *turc;

Servo *myservo;

void setup() {
  Serial.begin(115200);

  steppers = new MultiStepper();
  myservo = new Servo();
  turc = new TURC(steppers, myservo);

  turc->begin();

  turc->configureMicrostepping(false, false, false);

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  myservo->setPeriodHertz(50);    // standard 50 hz servo
	myservo->attach(SERVO, 1000, 2000);

  // turc->homing();

  turc->stepper1->setMaxSpeed(1000.0);
  turc->stepper1->setAcceleration(1000.0);
  turc->stepper1->moveTo(-1000);
  
  turc->stepper2->setMaxSpeed(1000.0);
  turc->stepper2->setAcceleration(1000.0);
  turc->stepper2->moveTo(1000);

  // long positions[4];
  // positions[0] = STEPS_PER_REVOLTION;
  // positions[1] = STEPS_PER_REVOLTION;
  // positions[2] = 0;
  // positions[4] = 0;
  
  // steppers->moveTo(positions);
  Position pos = {0.1, 0, 0};
  turc->moveTo(pos);

}

void loop() {
  // turc->RxManage();

  // turc->machine();

  // long positions[3];
  // positions[0] = 200;
  // positions[1] = 200;
  // positions[2] = 200;
  
  // steppers->moveTo(positions);
  // steppers->runSpeedToPosition();
  // delay(1000);
  // positions[0] = 0;
  // positions[1] = 0;
  // positions[2] = 0;

  // steppers->moveTo(positions);
  steppers->run();

  //   if (turc->stepper2->distanceToGo() == 0)
	// turc->stepper2->moveTo(-turc->stepper2->currentPosition());
  //   turc->stepper1->run();
  //   turc->stepper2->run();
  // Serial.println(turc->stepper1->currentPosition());
}
