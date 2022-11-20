#include <AFMotor.h>
#include <NewPing.h>
#include <Circuum.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define RELAY_PIN A2

#define MAX_DISTANCE 300
#define MIN_DISTANCE 16

#define MOTOR_SPEED 32 // Actual Speed
#define TURN_SPEED 10

// AF_DCMotor motor_left(4);
// NewPing sonar2(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Circuum crm;

boolean forwarding = false;
boolean triggered = false;
boolean reDecide = false;

int ran_int = 0;
int prev_read = 0;
int curr_read = 0;

int collision_streak_count = 0;

unsigned long upTime = 0;
unsigned long lastCollision = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Set Initial Speed
  crm.moveStop();
  crm.setSpeeds(MOTOR_SPEED);
  delay(2000);
  crm.moveForward(MOTOR_SPEED);
}

bool isOBSTACLE() {
  int distance = crm.readDistance();

  // return true if
  if (distance <= MIN_DISTANCE) {
    return true;
  } else {
    return false;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  upTime = millis();

  if (triggered) {
    prev_read = curr_read;
    curr_read = crm.readDistance();

    if (curr_read < prev_read && reDecide == false) {
      reDecide = true;
    }

    if (curr_read < 30) {
      if (collision_streak_count == 3) {
        collision_streak_count = 0;
        crm.moveBackward(MOTOR_SPEED + TURN_SPEED, 500);
        crm.moveStop();
        delay(1000);
      }

      if (ran_int < 2) {
        if (reDecide) {
          crm.moveRight(MOTOR_SPEED + TURN_SPEED);
        } else {
          crm.moveLeft(MOTOR_SPEED + TURN_SPEED);
        }
      } else {
        if (reDecide) {
          crm.moveLeft(MOTOR_SPEED + TURN_SPEED);
        } else {
          crm.moveRight(MOTOR_SPEED + TURN_SPEED);
        }
      }
    } else {
      triggered = false;
      crm.moveStop();
    }
  } else if (isOBSTACLE()) {
    forwarding = false;
    triggered = true;

    if (millis() - lastCollision < 2000) {
      collision_streak_count++;
    }
    lastCollision = millis();

    crm.moveStop();
    delay(300);

  } else {
    if (!forwarding) {
      forwarding = true;
      triggered = false;

      crm.moveForward(MOTOR_SPEED);
      ran_int = random(0, 4);
    }
  }
  delay(10);
}