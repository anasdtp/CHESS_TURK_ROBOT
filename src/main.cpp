#include <Arduino.h>
#include <GPIO.h>
#include <TURC.h>

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper *steppers;
TURC *turc;

Servo *myservo;

void setup() {
  Serial.begin(921600);

  steppers = new MultiStepper();
  myservo = new Servo();
  turc = new TURC(steppers, myservo);

  turc->begin();

  turc->configureMicrostepping(false, false, false);

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

	myservo->attach(SERVO);

  // turc->homing();

  turc->stepper1->setMaxSpeed(1000.0);
  turc->stepper1->setAcceleration(1000.0);
  turc->stepper1->moveTo(1000);
  turc->stepper1->setCurrentPosition(0);
  
  turc->stepper2->setMaxSpeed(1000.0);
  turc->stepper2->setAcceleration(1000.0);
  turc->stepper2->moveTo(1000);
  turc->stepper2->setCurrentPosition(0);

  turc->stepper3->setMaxSpeed(1000.0);
  turc->stepper3->setAcceleration(1000.0);
  turc->stepper3->moveTo(1000);
  turc->stepper3->setCurrentPosition(0);

  // long positions[4];
  // positions[0] = STEPS_PER_REVOLTION;
  // positions[1] = STEPS_PER_REVOLTION;
  // positions[2] = 0;
  // positions[3] = 0;
  
  // steppers->moveTo(positions);
  Position pos = {0.1, 0, 0};
  turc->moveTo(pos);

  turc->sendMsg(ID_ACK_GENERAL);

}

unsigned long lastTime = 0;

void loop() {
  turc->RxManage();

  turc->machine();

  // if(millis() - lastTime > 1000) {
  //   lastTime = millis();
  //   turc->sendMsg(ID_ACK_SEND_CURRENT_POSITION, turc->getCurrentPosition());
  // }
}
