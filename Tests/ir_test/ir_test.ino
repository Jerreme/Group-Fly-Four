#define left A3
#define right A4

void setup() {
  // put your setup code here, to run once:
  pinMode(left, INPUT);
  pinMode(right, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("left: ");
  Serial.print(digitalRead(left));
  Serial.print(" | right: ");
  Serial.println(digitalRead(right));

  delay(500);
}
