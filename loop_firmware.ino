const int TRIGGER_PIN = 8;

unsigned int counter = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("The glitch reset the chip!");
}

void loop() {
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  counter++;
  Serial.println(counter);
}
