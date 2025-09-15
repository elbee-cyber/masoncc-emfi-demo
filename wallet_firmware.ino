#include <LiquidCrystal.h>

const char *SECRET = "apple badge pen flag pocket time school";
const int TRIGGER_PIN = 8;

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

int unlocked = 0;
void unlock(){
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Unlocked!");
  lcd.setCursor(0, 1);
  lcd.print("Balance:1337 BTC");
  Serial.print("You unlocked the wallet!");
}

void loop() {
  static char buffer[40];
  static int idx = 0;

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      buffer[idx] = '\0';
      idx = 0;

      digitalWrite(TRIGGER_PIN, HIGH);
      digitalWrite(TRIGGER_PIN, LOW);

      bool ok = (strcmp(buffer, SECRET) == 0);

      if (ok) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Unlocked!");
        lcd.setCursor(0, 1);
        lcd.print("Balance:1337 BTC");
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
    }
  }
  if(unlocked){
    unlock();
  }
}