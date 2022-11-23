#include <Circuum.h>
Circuum circuum(true);  // On = DebugMode (displays reading to serial monitor)
// CircuumTest tests; // Uncomment this line if you want to perform tests

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);

  circuum.init();   // Initialize Circuum
  delay(3000);  // Boot Time
}

void loop() {
  circuum.AUTOMATIC_MODE();

  // Uncomment "SHUTDOWN()" if yuo want the robot automatic mode to be turned off 
  // (note:) the arduino board is still turned On, only the Automatic mode is turned off
  // circuum.SHUTDOWN();

}