#include <AFMotor.h>
#include <NewPing.h>
#include <Circuum.h>

#define TRIG_PIN A0
#define ECHO_PIN A1

#define IR_PIN A4
#define RELAY_PIN A5

// #define MAX_DISTANCE 400
#define MIN_DISTANCE 12
#define TURNING_DISTANCE 24

#define MOTOR_SPEED 35  // Actual Speed
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
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);

  digitalWrite(RELAY_PIN, false);

  //Set Initial Speed
  crm.moveStop();
  crm.setSpeeds(MOTOR_SPEED);
  delay(2000);
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

void wait(unsigned int time) {
  delay(time);
}

void testULTRA() {
  crm.readDistances();
  wait(200);
}

void loop() {
  crm.readDistances();

  bool stair = isStair();
  Serial.println(stair);
  millis();

  // |--------------| Check Stairs |--------------
  // if (!stair) {
  //   crm.turnPump(false);
  //   crm.moveBackward(MOTOR_SPEED + TURN_SPEED, 600);
  //   crm.moveStop();
  //   delay(400);

  //   crm.readDistances();
  //   if (crm.LEFT_DISTANCE > crm.RIGHT_DISTANCE) {
  //     crm.moveLeft(MOTOR_SPEED + TURN_SPEED);
  //     wait(700);
  //   } else {
  //     crm.moveRight(MOTOR_SPEED + TURN_SPEED);
  //     wait(700);
  //   }
  //   return;
  // }

  if (triggered) {
    crm.turnPump(false);
    if (crm.LEFT_DISTANCE < TURNING_DISTANCE || crm.RIGHT_DISTANCE < TURNING_DISTANCE) {  // Turning either sides
      if (crm.LEFT_DISTANCE >= crm.RIGHT_DISTANCE - 3 && crm.LEFT_DISTANCE <= crm.RIGHT_DISTANCE + 3) {
        crm.moveBackward(MOTOR_SPEED + TURN_SPEED, 600);
        // wait(400);

        crm.readDistances();
        if (crm.LEFT_DISTANCE > crm.RIGHT_DISTANCE) {
          crm.moveLeft(MOTOR_SPEED + TURN_SPEED);
          wait(700);
        } else {
          crm.moveRight(MOTOR_SPEED + TURN_SPEED);
          wait(700);
        }

        triggered = false;
        return;
      }

      if (crm.LEFT_DISTANCE > crm.RIGHT_DISTANCE) {
        crm.moveLeft(MOTOR_SPEED + TURN_SPEED);
      } else if (crm.RIGHT_DISTANCE > crm.LEFT_DISTANCE) {
        crm.moveRight(MOTOR_SPEED + TURN_SPEED);
      }
    } else {
      triggered = false;
      crm.moveStop();
      delay(100);
    }
  } else if (crm.LEFT_DISTANCE < MIN_DISTANCE || crm.RIGHT_DISTANCE < MIN_DISTANCE) {
    forwarding = false;
    triggered = true;

    crm.moveStop();
    delay(200);
  } else {
    if (!forwarding) {
      forwarding = true;
      triggered = false;


      crm.turnPump(true);
      crm.moveForward(MOTOR_SPEED);
    }
  }
  delay(10);
}


bool isStair() {
  return !digitalRead(IR_PIN);
}