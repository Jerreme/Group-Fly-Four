// ------| Libararies |------ 
#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

// ------| Pins |------ 
#define TRIG_PIN A0
#define ECHO_PIN A1

#define IR_PIN_1 A3  // left IR //NEW
#define IR_PIN_2 A4  // right IR //NEW
#define RELAY_PIN A2 // relay

// ------| Preferences |------ 
#define MAX_DISTANCE 200 // DEFAUTL 200
#define MAX_SPEED 75  // sets speed of DC  motors DEFAULT 70
#define MAX_SPEED_OFFSET 20 //DEFAULT 20


// ------| Declarations |------ 
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

// ------| Global Variables |------ 
boolean goesForward = false;
int distance = 100;
// int speedSet = 0;

bool left_stair = false; //NEW
bool right_stair = false; //NEW

// ------| setup |------ 
void setup() {
  pinMode(IR_PIN_1, INPUT); //NEW
  pinMode(IR_PIN_2, INPUT); //NEW
  
  pinMode(RELAY_PIN, OUTPUT); //NEW

  myservo.attach(10);
  myservo.write(115);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

// ------| loop |------ 
void loop() {

  int distanceR = 0;
  int distanceL = 0;
  delay(40);

  if (distance <= 15) {
    digitalWrite(RELAY_PIN, HIGH); //NEW
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if (distanceR >= distanceL) {
      turnRight();
      turnRight();
      moveStop();
    } else {
      turnLeft();
      turnLeft();
      moveStop();
    }
  } else {
    moveForward();
    digitalWrite(RELAY_PIN, LOW); //NEW
  }

  if (left_stair || right_stair) { //NEW
    digitalWrite(RELAY_PIN, HIGH); //NEW
    moveStop();
    delay(50);
    moveBackward();
    delay(300);
    moveStop();
    delay(100);
    if (left_stair) { //NEW
      turnLeft();
      turnLeft();
      moveStop();
    }else if (right_stair) { //NEW
      turnRight();
      turnRight();
      moveStop();
    }
  }
  
  distance = readPing();
  look_stair(); //NEW
}

void look_stair() { //NEW
  left_stair = digitalRead(IR_PIN_1);
  right_stair = digitalRead(IR_PIN_2);
}

int lookRight() {
  myservo.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115);
  return distance;
}

int lookLeft() {
  myservo.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115);
  return distance;
  delay(100);
}

int readPing() {
  delay(50);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 200;
  }
  return cm;
}

void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {

  if (!goesForward) {
    goesForward = true;
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    for (int speedSet = 0; speedSet < MAX_SPEED; speedSet += 2)  // slowly bring the speed up to avoid loading down the batteries too quickly
    {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(5);
    }
  }
}

void moveBackward() {
  goesForward = false;
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  for (int speedSet = 0; speedSet < MAX_SPEED; speedSet += 2)  // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(350);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(350);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
