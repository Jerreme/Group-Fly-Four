#include <Circuum.h>
Circuum circuum(true);  // On = DebugMode (displays reading to serial monitor)

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);

  digitalWrite(RELAY_PIN, LOW);  // Pulled Low Vacuum Relay

  circuum.init();   // Initialize Circuum
  delay(3000);  // Boot Time
}

void loop() {
  circuum.AUTOMATIC_MODE();
}