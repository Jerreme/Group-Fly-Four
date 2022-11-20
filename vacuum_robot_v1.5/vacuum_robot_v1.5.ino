// ------| Libararies |------
#include <AFMotor.h>
// #include <NewPing.h>
#include <Servo.h>
#include <VacuumTask.h>  //vcr library

// ------| Pins |------
// #define TRIG_PIN A0
// #define ECHO_PIN A1

#define IR_PIN_1 A3  // left IR
#define IR_PIN_2 A4  // right IR

#define RELAY_PIN A2   // relay
#define SERVO1_PIN 10  // servo 1 // 10

// ------| Preferences |------
#define MIN_DISTANCE 20         // DEFAULT 200
#define MAX_DISTANCE 200        // DEFAULT 200
#define TIME_PER_20_DEGREE 400  

#define TURN_SPEED 90           // lEFT, RGHT SPEED
#define BACKWARD_SPEED 120      // BACKWARD SPEED

// #define MAX_SPEED 75         // MOTOR SPEED
// #define MAX_SPEED_OFFSET 20  //DEFAULT 20


// |------------ Declarations |------------
// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;
VacuumTask vcr;

// AF_DCMotor motor1(1, MOTOR12_1KHZ);
// AF_DCMotor motor2(2, MOTOR12_1KHZ);
// AF_DCMotor motor3(3, MOTOR34_1KHZ);
// AF_DCMotor motor4(4, MOTOR34_1KHZ);

// -----------| Global Variables |------------
// boolean goesForward = false;
// int distance = 100;
// int speedSet = 0;

bool left_stair = false;
bool right_stair = false;

int OBSTACLE_DISTANCE = 0;
int index_left = 0;
int index_right = 0;
int highest_left = 0;
int highest_right = 0;

// ------------| setup |------------
void setup() {
  pinMode(IR_PIN_1, INPUT);
  pinMode(IR_PIN_2, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  myservo.attach(SERVO1_PIN);
  myservo.write(115);

  // OBSTACLE_DISTANCE = vcr.readDistance(2);
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000);  //2000
  Serial.begin(9600);
}



// ------------| loop |-----------
void loop() {
  
  if (isOBSTACLE()) {  // Obstacle
  // if (isOBSTACLE() | isSTAIR()) {  // Obstacle
    vcr.turnVacuum(LOW);
    vcr.moveBackward(BACKWARD_SPEED, 200);

    // if (left_stair) {
    //   vcr.turnLeft(600);
    //   return;
    // } else if (right_stair) {
    //   vcr.turnRight(600);
    //   return;
    // }

    if (lookLEFT() > lookRIGHT()) {
      // vcr.turnLeft(200);
      vcr.turnLeft(TURN_SPEED ,(index_left+1) * TIME_PER_20_DEGREE);
    } else {
      // vcr.turnRight(200);
      vcr.turnRight(TURN_SPEED, (index_right+1) * TIME_PER_20_DEGREE);
    }

    // if (lookRight >= lookLeft) {
    //   vcr.turnRight(350);
    // } else {
    //   vcr.turnLeft(350);
    // }
  
  } else {
    vcr.moveForward();
    vcr.turnVacuum(HIGH);
  }

  delay(300);
}

// ------------| Functions | ------------
bool isOBSTACLE() {
  int OBSTACLE_DISTANCE2 = vcr.readDistance();

  if (OBSTACLE_DISTANCE2 <= 20) {
    return true;
  } else {
    return false;
  }
}

bool isSTAIR() {
  left_stair = digitalRead(IR_PIN_1);
  right_stair = digitalRead(IR_PIN_2);

  if (left_stair || right_stair) {
    return true;
    Serial.println("STAIR TRUE");
  } else {
    return false;
  }
}

int lookRight() {
  myservo.write(50);
  delay(400);
  myservo.write(115);
  return vcr.readDistance();
}

int lookLeft() {
  myservo.write(170);
  delay(400);
  myservo.write(115);
  return vcr.readDistance();
}


// -------------------------| NEW FEATURES -------------------------|
unsigned int lookRIGHT() {
  int distances[3];
  int i = 0;

  for (int current = myservo.read(); current >= 115 - 60; current -= 20) {
    if (current == 115) {
      continue;
    }
    Serial.print("Angle: ");
    Serial.print(current);
    Serial.print(" | ");

    myservo.write(current);
    delay(200);  // Fix Interval
    distances[i++] = vcr.readDistance();
  }

  myservo.write(115);
  delay(300);
  highest_right = getHeighestFromArray(distances, 3, index_right);
  return highest_right;
}

unsigned int lookLEFT() {
  int distances[3];
  int i = 0;

  for (int current = myservo.read(); current <= 115 + 60; current += 20) {
    if (current == 115) {
      continue;
    }
    Serial.print("Angle: ");
    Serial.print(current);
    Serial.print(" | ");

    myservo.write(current);
    delay(200);  // Fix Interval
    distances[i++] = vcr.readDistance();
  }

  myservo.write(115);
  delay(300);
  highest_left = getHeighestFromArray(distances, 3, index_left);
  return highest_left;
}

unsigned int getHeighestFromArray(int myArray[], int size, int &index) {
  int maxVal = myArray[0];
  index = 0;
  for (int i = 0; i < size; i++) {
    if (myArray[i] > maxVal) {
      maxVal = myArray[i];
      index = i;
    }
  }
  return maxVal;
}