#include <Circuum.h>

Circuum crm(true);  // On DebugMode (displays reading to serial monitor)

#define IR_PIN A4
#define RELAY_PIN A5

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);

  digitalWrite(RELAY_PIN, LOW);  // Pulled Low Vacuum Relay

  crm.init();   // Initialize Circuum
  delay(3000);  // Boot Time
}

void loop() {
  crm.AUTOMATIC_MODE();
}