#define relay A5

void setup() {
  // put your setup code here, to run once:
  pinMode(relay, OUTPUT);

  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(relay, true);
  delay(1000);
  digitalWrite(relay, false);
  delay(1000);
}
