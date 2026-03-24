#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD with 16 columns x 2 rows

const int trigPin = 9;
const int echoPin = 10;

const int lowLED = 2;
const int midLED = 3;
const int highLED = 4;
const int relayPin = 5;  // Relay connected to digital pin 5

int mapmax = 30;

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(lowLED, OUTPUT);
  pinMode(midLED, OUTPUT);
  pinMode(highLED, OUTPUT);
  pinMode(relayPin, OUTPUT);  // Set the relay pin as an output

  Serial.begin(9600);
}

void loop() {
  lcd.clear();

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  int mapped = map(distance, 5, mapmax, 100, 0);

  if (mapped > 0) {
    Serial.print("Tank Level: ");
    Serial.print(mapped);
    Serial.println("%");
  } else {
    Serial.println("OUT Of Range");
  }

  // Set cursor to the first row to display percentage and status
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(mapped); // Print percentage value
  lcd.print("% ");

  // Determine the water level status (High, Mid, Low)
  if (mapped >= 75) {
    lcd.print("High");
    digitalWrite(lowLED, LOW);
    digitalWrite(midLED, LOW);
    digitalWrite(highLED, HIGH);
    digitalWrite(relayPin, LOW);  // Turn off pump when water level is high
  } else if (mapped >= 30 && mapped < 75) {
    lcd.print("Mid");
    digitalWrite(lowLED, LOW);
    digitalWrite(midLED, HIGH);
    digitalWrite(highLED, LOW);
  } else if (mapped > 0 && mapped < 30) {
    lcd.print("Low");
    digitalWrite(lowLED, HIGH);
    digitalWrite(midLED, LOW);
    digitalWrite(highLED, LOW);
    digitalWrite(relayPin, HIGH);  // Turn on pump when water level is low
  } else {
    lcd.print("Out");
    digitalWrite(lowLED, LOW);
    digitalWrite(midLED, LOW);
    digitalWrite(highLED, LOW);
    digitalWrite(relayPin, LOW);  // Turn off pump when out of range
  }

  // Gradual graph representation on the second row
  int blocks = mapped / 10; // Calculate number of blocks to display (10% = 1 block)
  lcd.setCursor(0, 1);

  for (int i = 0; i < blocks; i++) {
    lcd.print("|"); // Use '|' as a block, can replace with other characters like '█'
  }
  for (int i = blocks; i < 10; i++) {
    lcd.print(" "); // Fill the rest of the row with spaces
  }

  delay(500);
}
