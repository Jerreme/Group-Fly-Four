#include "VacuumTask.h"
#include <NewPing.h>
#include <AFMotor.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define RELAY_PIN A2

#define MAX_DISTANCE 200

#define MAX_SPEED 50
#define MAX_SPEED_OFFSET 20

#define IR_PIN_1 A3 // left IR
#define IR_PIN_2 A4 // right IR

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

NewPing sonar2(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
boolean goesForward = false;

unsigned int VacuumTask::readDistance(unsigned int count)
{
    if (count < 1)
        count = 1;

    for (int i = 0; i < count; i++)
    {
        unsigned int reading = sonar2.ping_cm(); // Send ping, get ping time in microseconds (uS).
        delay(40);                               // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

        if (reading == 0)
            reading = MAX_DISTANCE;

        Serial.print("Distance: ");
        Serial.print(reading);
        Serial.println("cm");
        return reading;
    }
}
unsigned int VacuumTask::readDistance()
{
    return readDistance(1);
}

// VacuumTask::

void VacuumTask::moveStop()
{
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
}

void VacuumTask::moveForward()
{
    if (!goesForward)
    {

        goesForward = true;
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);

        forwardTurbo();
    }
}

void VacuumTask::moveBackward(unsigned int speed, unsigned int duration)
{
    moveStop();
    goesForward = false;

    // graduallyDecreaseSpeed();
    setAllMotorSpeed(speed);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);

    delay(duration);
    moveStop();
}

void VacuumTask::setAllMotorSpeed(int speed)
{
    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
    motor3.setSpeed(speed);
    motor4.setSpeed(speed);
}

void VacuumTask::forwardTurbo()
{
    // for (int speedSet = 90; speedSet > MAX_SPEED; speedSet -= 1)
    // {
    //     setAllMotorSpeed(speedSet);
    //     Serial.println("hahaha: ");
    //     Serial.println(speedSet); // 70
    //     delay(20);
    // }

    setAllMotorSpeed(100);
    delay(200);
    setAllMotorSpeed(MAX_SPEED);
}

void VacuumTask::turnRight(unsigned int speed, unsigned int duration)
{
    setAllMotorSpeed(speed);

    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
    delay(duration);

    // moveForward();
    moveStop();

    setAllMotorSpeed(MAX_SPEED);
}

void VacuumTask::turnLeft(unsigned int speed, unsigned int duration)
{
    setAllMotorSpeed(speed);

    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    delay(duration);

    // moveForward();
    moveStop();

    setAllMotorSpeed(MAX_SPEED);
}

void VacuumTask::turnVacuum(bool status)
{
    // digitalWrite(RELAY_PIN, !status); // NEW
}