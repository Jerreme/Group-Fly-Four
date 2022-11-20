#include "Circuum.h"
#include <NewPing.h>
#include <AFMotor.h>

#define TRIG_PIN A0
#define ECHO_PIN A1

#define TRIG_PIN2 A2
#define ECHO_PIN2 A3

#define RELAY_PIN A5

#define MAX_DISTANCE 400
#define RIGHT_ADJUSTMENT 25

NewPing ultraLeft(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing ultraRight(TRIG_PIN2, ECHO_PIN2, MAX_DISTANCE);

AF_DCMotor left_motor(1);
AF_DCMotor right_motor(4);

void Circuum::readDistances()
{
    LEFT_DISTANCE = ultraLeft.ping_cm(); 
    RIGHT_DISTANCE = ultraRight.ping_cm();

    delay(50);
    if (LEFT_DISTANCE == 0)
        LEFT_DISTANCE = MAX_DISTANCE;
    if (RIGHT_DISTANCE == 0)
        RIGHT_DISTANCE = MAX_DISTANCE;

    
    Serial.print(LEFT_DISTANCE);
    Serial.print(" | ");
    Serial.println(RIGHT_DISTANCE);

    // return reading;
}

unsigned int Circuum::readDistance()
{
    unsigned int reading = ultraRight.ping_cm(); // Send ping, get ping time in microseconds (uS).
    delay(40);                              // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

    if (reading == 0)
        reading = MAX_DISTANCE;

    Serial.print("Distance Right: ");
    Serial.println(reading);
    return reading;
}

void Circuum::setSpeeds(int speed)
{
    left_motor.setSpeed(speed);
    right_motor.setSpeed(speed+RIGHT_ADJUSTMENT);

    // forwardTurbo();
}

void Circuum::forwardTurbo(int speed)
{
    setSpeeds(speed+30);
    delay(120);
    setSpeeds(speed);;
}

void Circuum::moveForward(int speed)
{
    setSpeeds(speed);
    left_motor.run(FORWARD);
    right_motor.run(FORWARD);

    forwardTurbo(speed);
}



void Circuum::moveLeft(int speed)
{
    setSpeeds(speed);
    left_motor.run(BACKWARD);
    right_motor.run(FORWARD);

    // forwardTurbo();
}

void Circuum::moveRight(int speed)
{
    setSpeeds(speed);
    left_motor.run(FORWARD);
    right_motor.run(BACKWARD);

    // forwardTurbo();
}

void Circuum::moveStop()
{
    left_motor.run(RELEASE);
    right_motor.run(RELEASE);
}

void Circuum::moveBackward(int speed, int time)
{
    setSpeeds(speed);
    left_motor.run(BACKWARD);
    right_motor.run(BACKWARD);

    delay(time);

    moveStop();
    // forwardTurbo();
}

void Circuum::turnPump(bool state) {
    digitalWrite(RELAY_PIN, state);
}