#include <Servo.h>
#include <VacuumTask.h>

#define SERVO_PIN 9  // Servo
Servo myservo;
VacuumTask vcr;

int index_left = 0;
int index_right = 0;
int highest_left =0;
int highest_right =0;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(9);
  Serial.begin(9600);

  myservo.write(115);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // serial_servo_control();
   auto_mode();

//  if (isOBSTACLE()) {
//    if (lookLEFT() > lookRIGHT()) {
//      Serial.println(highest_left);
//    } else {
//      Serial.println(highest_right);
//    }
//
//    Serial.print("left----------- ");
//    Serial.println(index_left);
//    Serial.print("right----------- ");
//    Serial.println(index_right);
//  }
//  delay(200);
}


bool isOBSTACLE() {
  int OBSTACLE_DISTANCE = vcr.readDistance();

  if (OBSTACLE_DISTANCE < 20) {
    return true;
  } else {
    return false;
  }
}

unsigned int lookRIGHT() {
  int distances[3];
  int i = 0;

  // Look Right
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

  // Look Left
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















void serial_servo_control() {
  if (Serial.available()) {
    int read = Serial.parseInt();

    if (read > 0) {
      myservo.write(read);

      Serial.print("rotated to: ");
      Serial.println(read);


      delay(100);
      int OBSTACLE_DISTANCE = vcr.readDistance();
    }
  }
}

void auto_mode() {
  myservo.write(random(30, 160));
  delay(random(100, 900));
}
