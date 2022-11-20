
#include <AFMotor.h>

AF_DCMotor motorL(1);
AF_DCMotor motorR(4);

#define MOTORSPEED 32

void setup() {
  // turn on motor
  motorL.setSpeed(MOTORSPEED);
  motorR.setSpeed(MOTORSPEED+25); //40

  motorL.run(RELEASE);
  motorR.run(RELEASE);

  delay(2000);
  motorL.run(FORWARD);
  motorR.run(FORWARD);
}

void loop() {
}