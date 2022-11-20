#ifndef VacuumTask_h
#define VacuumTask_h

#include "Arduino.h"
#define VacuumTask_LIB_VERSION "0.1.0"

class VacuumTask
{
private:
    /* data */
    uint8_t TRIG_PIN;
    uint8_t ECHO_PIN;
    uint8_t MAX_DISTANCE;

    void setAllMotorSpeed(int speed);

public:
    unsigned int readDistance(unsigned int);
    unsigned int readDistance();

    void moveStop();
    void forwardTurbo();
    void moveForward();
    void moveBackward(unsigned int speed, unsigned int duration);

    void turnRight(unsigned int speed, unsigned int duration);
    void turnLeft(unsigned int speed, unsigned int duration);

    void turnVacuum(bool status);
};

#endif
