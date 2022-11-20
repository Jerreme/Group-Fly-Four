#define ir1 A3
#define ir2 A4

void setup() {
  // put your setup code here, to run once:
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(ir1));
  Serial.print(" | ");
  Serial.println(digitalRead(ir2));

  delay(500);
}
