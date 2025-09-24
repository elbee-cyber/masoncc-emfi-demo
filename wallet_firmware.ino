#include <LiquidCrystal.h>
#define TRIGGER_PIN = 8

const char secret[17] = "demo seed phrase";
int unlocked = 0;

// LCD: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Patriot Wallet");
  lcd.setCursor(0, 1);
  lcd.print("Enter seed >>");
}

void unlock(){
  delay(500);
  Serial.print("Welcome back trusted user!");
  lcd.clear();
  lcd.setCursor(0, 0);
  // This wallet is completely empty and were generated for this challenge
  lcd.print(" BTC: 1EdcxBwCCnFEnHrhDmVQ9vvk8wwksfDD8w");
  lcd.setCursor(0, 1);
  lcd.print(" SECRET: L1faYUqikWEo4cWMbRAY2vanGCmMZJ1AhjP4TJTHrwYuXj4wkQ2U");
  while(1){delay(500);lcd.scrollDisplayLeft();}
}

void loop() {
  static char buffer[17];
  static int idx = 0;
  
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      buffer[idx] = '\0';

      digitalWrite(TRIGGER_PIN, HIGH);
      digitalWrite(TRIGGER_PIN, LOW);

      bool ok = (strcmp(buffer, SECRET) == 0);

      if (ok) {
        lcd.clear();
        Serial.print("1");
        unlocked = 1;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");
        lcd.setCursor(0, 1);
        lcd.print("Please try again");
        Serial.print("0");
      }
      idx = 0;
    } else if (idx < 17){
      buffer[idx++] = c;
    }
  }
  if (unlocked){
    unlock();
  }
}
