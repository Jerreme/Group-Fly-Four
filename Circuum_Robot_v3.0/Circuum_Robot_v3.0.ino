#include <Circuum.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);  // RX, TX

Circuum circuum(true);  // On = DebugMode (displays reading to serial monitor)
// CircuumTest tests;      // Uncomment this line if you want to perform tests

String START = "START";
String STOP = "STOP";
bool ON = false;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  Serial.println("Beginning Circuum...");
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);

  circuum.init();  // Initialize Circuum
  // delay(3000);     // Boot Time
}
               
void loop() {
  if (ON) {
    circuum.AUTOMATIC_MODE();
  }

  if (mySerial.available()) {
    String command = mySerial.readString();
    command.trim();
    // command = command.replace("\n","");
    // command = command.trim();
    
    Serial.println(command);
    if (command.equals(START)) {
      ON = true;
      Serial.println("Circuum has Started.");
    } else {
      ON = false;
      circuum.SHUTDOWN();
      Serial.println("Circuum has Stopped.");
    }
  }
}
