#ifndef Circuum_h
#define Circuum__h

#include "Arduino.h"
#define Circuum_LIB_VERSION "0.1.1"

class Circuum
{
private:
    // Variables
    bool DEBUGMODE = true;
    bool FORWARDING = false;

    bool COLLIDE = false;
    bool CLIFF = false;
    bool STUCKED = false;

    int collision_streak_count = 0;
    const int collision_streak_trigger = 3;

    unsigned long lapTime_collision = 0;
    unsigned long lapTime_forwarding = 0;

    unsigned short LEFT_DISTANCE;
    unsigned short RIGHT_DISTANCE;

    unsigned short PREV_LEFT_DISTANCE = 0;
    unsigned short PREV_RIGHT_DISTANCE = 0;

    bool lookCliff();
    void readDistances();

    void setSpeeds(unsigned char speed);
    void moveStop(unsigned char stopTime);
    void Turbo(unsigned char speed);
    void turnPump(bool state);

    void moveForward(unsigned char speed);
    void moveBackward(unsigned char speed, unsigned short backwardTime);
    void moveLeft(unsigned char speed);
    void moveRight(unsigned char speed);

    void SCAN();
    void EVALUATE();
    void DECISION();

    void ENCOUNTERED_WALL();
    void ENCOUNTERED_LEFT_OBSTACLE();
    void ENCOUNTERED_RIGHT_OBSTACLE();
    void ENCOUNTERED_CLIFF_OBSTACLE();
    void ENCOUNTERED_STUCK_OBSTACLE();

public:
    void init(unsigned short milliseconds);
    void AUTOMATIC_MODE();

    void println(String message);

    void testULTRA();
    void testDYNAMO(unsigned char DIRECTION);
    void testIR();
};


#endif
