#ifndef Circuum_h
#define Circuum__h

#include "Arduino.h"
#define Circuum_LIB_VERSION "0.1.2"

// UltraSonic Left
#define TRIG_PIN A0
#define ECHO_PIN A1
// UltraSonic Right
#define TRIG_PIN2 A2
#define ECHO_PIN2 A3

#define IR_PIN A4
#define RELAY_PIN A5

class Circuum
{
private:
    // Variables
    bool DEBUGMODE = true;
    bool FORWARDING = false;

    bool COLLIDE = false;
    bool CLIFF = false;
    bool STUCKED = false;
    bool VACUUM = false;

    int collision_streak_count = 0;
    const int collision_streak_trigger = 3;

    unsigned long lapTime_collision = 0;
    unsigned long lapTime_forwarding = 0;

    unsigned short PREV_LEFT_DISTANCE = 0;
    unsigned short PREV_RIGHT_DISTANCE = 0;

    void setSpeeds(unsigned char speed);
    void moveStop(unsigned short stopTime);
    void Turbo(unsigned char speed);
    void turnPump(bool state);

    void SCAN();
    void EVALUATE();
    void DECISION();

    void ENCOUNTERED_WALL();
    void ENCOUNTERED_LEFT_OBSTACLE();
    void ENCOUNTERED_RIGHT_OBSTACLE();
    void ENCOUNTERED_CLIFF_OBSTACLE();
    void ENCOUNTERED_STUCK_OBSTACLE();

public:
    Circuum(bool DebugMode);
    Circuum();

    void init();
    void AUTOMATIC_MODE();
    void SHUTDOWN();

    void println(String message);

protected:
    bool lookCliff();
    void readDistances();

    void moveForward(unsigned char speed);
    void moveBackward(unsigned char speed, unsigned short backwardTime);
    void moveLeft(unsigned char speed);
    void moveRight(unsigned char speed);

    unsigned short LEFT_DISTANCE;
    unsigned short RIGHT_DISTANCE;
};

#define MODE_BACKWARD 0
#define MODE_FORWARD 1
#define MODE_LEFT 6
#define MODE_RIGHT 7
#define MODE_STOP 9

class CircuumTest : public Circuum
{
public:
    void testULTRA();
    void testDYNAMO(unsigned char DIRECTION);
    void testIR();
};

#endif
