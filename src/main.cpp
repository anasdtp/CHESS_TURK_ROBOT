#include <Arduino.h>
#include <GPIO.h>
#include <STEPPER.h>
#include <ESP32Servo.h>




// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper *steppers;
STEPPER *turc;

Servo myservo;

void setup() {
  Serial.begin(115200);

  steppers = new MultiStepper();
  turc = new STEPPER(steppers);

  turc->configureMicrostepping(true, false, false);

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(SERVO, 1000, 2000);
}

void loop() {
  Position pos;
  pos.x = 0.1;
  pos.y = 0.1;
  pos.z = 0.1;
  turc->moveTo(pos);
  
  pos.x = 0;
  pos.y = 0;
  pos.z = 0;
  turc->moveTo(pos);
}
