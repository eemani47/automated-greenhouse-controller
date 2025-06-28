#include <LiquidCrystal.h>

// LCD: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Pins
const int tempPin = A0;
const int lightPin = A1;
const int fanPin = 9;
const int buzzerPin = 8;
const int blueLedPin = 10;
const int whiteLedPin = 11;

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Greenhouse Ready");
  delay(1500);
  lcd.clear();

  // Set pin modes
  pinMode(fanPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(whiteLedPin, OUTPUT);
}

void loop() {
  // --- Temperature reading (TMP36) ---
  int tempRaw = analogRead(tempPin);
  float voltage = tempRaw * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0;

  // --- Light reading (Phototransistor) ---
  int lightValue = analogRead(lightPin);  // 0 = dark, 1023 = bright

  // --- Fan control ---
  int fanSpeed = 0;
  if (temperatureC > 30) {
    fanSpeed = map(temperatureC, 30, 50, 100, 255); // Map temp to PWM
    fanSpeed = constrain(fanSpeed, 100, 255);
    analogWrite(fanPin, fanSpeed);
  } else {
    analogWrite(fanPin, 0);
  }

  // --- Buzzer alert ---
  if (temperatureC >= 40) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // --- Light indication ---
  if (lightValue < 300) {
    digitalWrite(blueLedPin, HIGH);     // It's dark
    digitalWrite(whiteLedPin, LOW);
  } else {
    digitalWrite(blueLedPin, LOW);
    digitalWrite(whiteLedPin, HIGH);    // It's bright
  }

  // --- LCD Display ---
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatureC, 1);
  lcd.print((char)223);  // ° symbol
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  lcd.print("Light: ");
  lcd.print(lightValue);
  lcd.print("     ");

  delay(500); // Short delay for stable readings
}