#include "Circuum.h"
#include <NewPing.h>
#include <AFMotor.h>

// DISTANCES
#define MIN_DISTANCE 12
#define MAX_DISTANCE 400
#define MIN_OFFSET_DISTANCE 24

NewPing ultraLeft(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing ultraRight(TRIG_PIN2, ECHO_PIN2, MAX_DISTANCE);
AF_DCMotor left_motor(1);
AF_DCMotor right_motor(4);

// SPEEDS
#define MOTOR_SPEED 32
#define TURN_ADDITIONAL_SPEED 15
#define TURBO_ADDITIONAL_SPEED 80
#define RIGHT_ADDITIONAL_SPEED 30 // 25 // Because Right Dynamo is Slower than Left

// TIME
#define BACKWARD_TIME 600
#define WALL_TURNING_TIME 800
#define ROTATE_180_TIME 1600
#define HESITATE_TIME 200

Circuum::Circuum(bool DebugMode)
{
    this->DEBUGMODE = DebugMode;
}

Circuum::Circuum()
{
    this->DEBUGMODE = true;
}

void Circuum::init()
{
    setSpeeds(MOTOR_SPEED); // Set Initial Speed
    moveStop(0);            // Force Stop Circuum
}

void Circuum::readDistances()
{
    LEFT_DISTANCE = ultraLeft.ping_cm();
    RIGHT_DISTANCE = ultraRight.ping_cm();

    delay(50);
    if (LEFT_DISTANCE == 0)
        LEFT_DISTANCE = MAX_DISTANCE;
    if (RIGHT_DISTANCE == 0)
        RIGHT_DISTANCE = MAX_DISTANCE;

    println("L: " + String(LEFT_DISTANCE) + " | R:" + String(RIGHT_DISTANCE));
}

bool Circuum::lookCliff()
{
    return digitalRead(IR_PIN);
}

void Circuum::setSpeeds(unsigned char speed)
{
    left_motor.setSpeed(speed);
    right_motor.setSpeed(speed + RIGHT_ADDITIONAL_SPEED);
}

void Circuum::Turbo(unsigned char speed)
{
    setSpeeds(speed + TURBO_ADDITIONAL_SPEED);
    delay(80);
    setSpeeds(speed);
}

void Circuum::moveForward(unsigned char speed)
{
    setSpeeds(speed);
    left_motor.run(FORWARD);
    right_motor.run(FORWARD);
    Turbo(speed);
}

void Circuum::moveLeft(unsigned char speed)
{
    setSpeeds(speed);
    left_motor.run(BACKWARD);
    right_motor.run(FORWARD);
}

void Circuum::moveRight(unsigned char speed)
{
    setSpeeds(speed);
    left_motor.run(FORWARD);
    right_motor.run(BACKWARD);
}

void Circuum::moveStop(unsigned char stopTime)
{
    left_motor.run(RELEASE);
    right_motor.run(RELEASE);

    if (stopTime != 0)
        delay(stopTime);
}

void Circuum::moveBackward(unsigned char speed, unsigned short backwardTime)
{
    setSpeeds(speed);
    left_motor.run(BACKWARD);
    right_motor.run(BACKWARD);

    delay(backwardTime);
    moveStop(0);
}

void Circuum::turnPump(bool state)
{
    digitalWrite(RELAY_PIN, state);
}

void Circuum::println(String message)
{
    if (DEBUGMODE)
        Serial.println(message);
}

// |--------------| AUTO MODE |--------------
void Circuum::AUTOMATIC_MODE()
{
    SCAN();
    EVALUATE();
    DECISION();
}

void Circuum::SCAN()
{
    readDistances(); // Ping Distances
    println("L: " + String(LEFT_DISTANCE) + " | R:" + String(RIGHT_DISTANCE));

    CLIFF = lookCliff(); // Ping Cliff
    println("Cliff: " + String(CLIFF));
}

void Circuum::EVALUATE()
{
    // Cliff Approaching
    if (CLIFF)
    {
        FORWARDING = false;
        moveStop(0);
        return;
    }

    // Check Position every 2 seconds
    if ((millis() % 2000) == 0)
    {
        STUCKED = false;

        if (PREV_LEFT_DISTANCE >= (LEFT_DISTANCE - 2) && PREV_LEFT_DISTANCE <= (LEFT_DISTANCE + 2))
            if (PREV_RIGHT_DISTANCE >= (RIGHT_DISTANCE - 2) && PREV_RIGHT_DISTANCE <= (RIGHT_DISTANCE + 2))
                STUCKED = true;

        PREV_LEFT_DISTANCE = LEFT_DISTANCE;
        PREV_RIGHT_DISTANCE = RIGHT_DISTANCE;
        return;
    }

    // Obstacle Approaching
    if (LEFT_DISTANCE < MIN_DISTANCE || RIGHT_DISTANCE < MIN_DISTANCE)
    {
        FORWARDING = false;
        COLLIDE = true;

        if ((millis() - lapTime_collision) <= 2000)
            collision_streak_count++;
        else
            collision_streak_count = 0;

        lapTime_collision = millis();
        moveStop(HESITATE_TIME);
        return;
    }
}

void Circuum::DECISION()
{
    // Cliff Detected, Execute Behavior
    if (CLIFF)
    {
        turnPump(false);
        ENCOUNTERED_CLIFF_OBSTACLE();

        CLIFF = false;
        moveStop(HESITATE_TIME);
    }

    // Circuum is stuck in a corner
    if (collision_streak_count == collision_streak_trigger)
    {
        turnPump(false);
        ENCOUNTERED_CLIFF_OBSTACLE();

        collision_streak_count = 0;
        moveStop(HESITATE_TIME);
    }

    // Circuum Stucked in a place
    if (STUCKED)
    {
        turnPump(false);
        ENCOUNTERED_STUCK_OBSTACLE();

        STUCKED = false;
        moveStop(HESITATE_TIME);
    }

    // Obstacle Detected, Execute Behavior
    while (COLLIDE)
    {
        SCAN();
        turnPump(false);

        if (LEFT_DISTANCE < MIN_OFFSET_DISTANCE || RIGHT_DISTANCE < MIN_OFFSET_DISTANCE)
        {

            // Wall Detected
            if (LEFT_DISTANCE >= RIGHT_DISTANCE - 2 && LEFT_DISTANCE <= RIGHT_DISTANCE + 2)
            {
                ENCOUNTERED_WALL();
                continue;
            }

            // Left Obstacke Detected
            if (LEFT_DISTANCE > RIGHT_DISTANCE)
            {
                ENCOUNTERED_LEFT_OBSTACLE();
                continue;
            }

            // Right Obstacke Detected
            else if (RIGHT_DISTANCE > LEFT_DISTANCE)
            {
                ENCOUNTERED_RIGHT_OBSTACLE();
                continue;
            }
        }
        else
        {
            COLLIDE = false;
            moveStop(HESITATE_TIME);
        }
    }

    // Keep Forwarding if No obstacle
    if (!FORWARDING)
    {
        FORWARDING = true;
        COLLIDE = false;
        CLIFF = false;

        // turnPump(true);
        lapTime_forwarding = millis();
        moveForward(MOTOR_SPEED);
    }
    else
    {
        if ((millis() - lapTime_forwarding) >= 1000)
        {
            turnPump(true);
        }
    }
}

// |--------------| BEHAVIORS |--------------
void Circuum::ENCOUNTERED_WALL()
{
    // moveBackward(MOTOR_SPEED + TURN_ADDITIONAL_SPEED, BACKWARD_TIME);
    if (LEFT_DISTANCE > RIGHT_DISTANCE)
        moveLeft(MOTOR_SPEED + TURN_ADDITIONAL_SPEED);
    else
        moveRight(MOTOR_SPEED + TURN_ADDITIONAL_SPEED);

    delay(WALL_TURNING_TIME);
}

void Circuum::ENCOUNTERED_LEFT_OBSTACLE()
{
    moveLeft(MOTOR_SPEED + TURN_ADDITIONAL_SPEED);
}

void Circuum::ENCOUNTERED_RIGHT_OBSTACLE()
{
    moveRight(MOTOR_SPEED + TURN_ADDITIONAL_SPEED);
}

void Circuum::ENCOUNTERED_CLIFF_OBSTACLE()
{
    moveBackward(MOTOR_SPEED + TURN_ADDITIONAL_SPEED, BACKWARD_TIME);
    SCAN();
    if (LEFT_DISTANCE < RIGHT_DISTANCE)
        moveLeft(MOTOR_SPEED + TURN_ADDITIONAL_SPEED);
    else
        moveRight(MOTOR_SPEED + TURN_ADDITIONAL_SPEED);

    delay(ROTATE_180_TIME);
}

void Circuum::ENCOUNTERED_STUCK_OBSTACLE()
{
    moveBackward(MOTOR_SPEED + TURBO_ADDITIONAL_SPEED, BACKWARD_TIME);
    SCAN();
    if (LEFT_DISTANCE < RIGHT_DISTANCE)
        moveLeft(MOTOR_SPEED + TURN_ADDITIONAL_SPEED);
    else
        moveRight(MOTOR_SPEED + TURN_ADDITIONAL_SPEED);

    delay(ROTATE_180_TIME);
}

// |--------------| Hardware Test |--------------

void CircuumTest::testULTRA()
{
    readDistances();
    println("L: " + String(LEFT_DISTANCE) + " | R:" + String(RIGHT_DISTANCE));
}

void CircuumTest::testDYNAMO(unsigned char DIRECTION)
{
    if (DIRECTION == MODE_FORWARD)
        moveForward(MOTOR_SPEED);
    else if (DIRECTION == MODE_BACKWARD)
        moveBackward(MOTOR_SPEED + TURN_ADDITIONAL_SPEED, BACKWARD_TIME);
    else if (DIRECTION == MODE_LEFT)
        moveLeft(MOTOR_SPEED);
    else if (DIRECTION == MODE_RIGHT)
        moveRight(MOTOR_SPEED);
}

void CircuumTest::testIR()
{
    println("IR: " + String(lookCliff()));
}