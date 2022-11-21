#include <Circuum.h>
Circuum crm;

#define IR_PIN A4
#define RELAY_PIN A5

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);

  digitalWrite(RELAY_PIN, LOW);

  crm.init(2500);
}

void loop() {
  crm.AUTOMATIC_MODE();
}