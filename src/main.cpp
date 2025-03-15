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
}

void loop() {
  // turc->RxManage();

  // turc->machine();

  long positions[3];
  positions[0] = 200;
  positions[1] = 200;
  positions[2] = 200;
  
  steppers->moveTo(positions);
  steppers->runSpeedToPosition();
  delay(1000);
  positions[0] = 0;
  positions[1] = 0;
  positions[2] = 0;

  steppers->moveTo(positions);
  steppers->runSpeedToPosition();


}
