#ifndef Circuum_h
#define Circuum__h

#include "Arduino.h"
#define Circuum_LIB_VERSION "0.1.0"

class Circuum
{
private:
public:
    void setSpeeds(int speed);
    unsigned int readDistance();
    void readDistances();

    void moveForward(int speed);
    void moveLeft(int speed);
    void moveRight(int speed);
    void moveStop();

    void moveBackward(int speed, int time);
    void forwardTurbo(int speed);
    
    void turnPump(bool state);

    int LEFT_DISTANCE;
    int RIGHT_DISTANCE;
};

#endif
