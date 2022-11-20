#include <NewPing.h>
#include <Servo.h>

#define SERVO_PIN 9  // Servo
Servo myservo;

#define TRIGGER_PIN 12                               // Orange
#define ECHO_PIN 11                                  // yellow
#define MAX_DISTANCE 400                             // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and maximum distance.


int pos = 0;
int servo_delay = 500;

void setup() {
  myservo.attach(9);

  myservo.write(0);
  delay(1000);
  Serial.begin(9600);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 20) {
    myservo.write(pos);
    delay(servo_delay);
    readDistance();
  }
  for (pos = 180; pos >= 0; pos -= 20) {
    myservo.write(pos);
    delay(servo_delay);
    readDistance();
  }
}

void readDistance() {              
  unsigned int uS = sonar.ping_cm();  // Send ping, get ping time in microseconds (uS).

  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  Serial.print(uS);  // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
}