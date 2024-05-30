#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Define pins for pushbuttons
const int buttonPins[] = {2, 3, 4, 5, 6};
const int numButtons = 5;

// Define angles corresponding to each button
const int angles[] = {180, 0, 150, 135, 0};

// Define messages corresponding to each button
const char *messages[] = {"VALVE OFF", "VALVE ON", "LOW HEAT", "MID HEAT", "HIGH HEAT"};

// Define pin for LEDs
const int ledPins[] = {13, 7, 8, 10, 11};

// Define pin for the buzzer
const int buzzerPin = 12;

// Define servo motor object
Servo gasValve;

// Define LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27, 16 columns and 2 rows

void setup() {
  // Initialize buzzer pin
  pinMode(buzzerPin, OUTPUT);

  // Play startup sound
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(30);
    digitalWrite(buzzerPin, LOW);
    delay(50);
  }

  // Initialize pushbuttons
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH);  // Turn on all LEDs initially
  }

  // Attach servo to pin 13
  gasValve.attach(9);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ARDUINO-BASED");
  delay(2000);
  lcd.clear ();
  
  lcd.print ("Gas Actuator");
  lcd.setCursor(3,1);
  lcd.print("System (AGAS)");
  delay(5000);
  
  // Turn off all LEDs
  for (int i = 0; i < numButtons; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  // Read the state of pushbuttons
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      // Button pressed, set servo angle, display message, and turn on LED
      gasValve.write(angles[i]);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(messages[i]);
      digitalWrite(ledPins[i], HIGH);
      
      // Beep the buzzer
      beep();
      
      // Turn off LEDs of other buttons
      for (int j = 0; j < numButtons; j++) {
        if (j != i) {
          digitalWrite(ledPins[j], LOW);
        }
      }
      
      // Wait for button release
      while (digitalRead(buttonPins[i]) == LOW) {
        delay(10); // Debounce delay
      }
    }
  }
}

void beep() {
  // Function to produce a beep sound
  digitalWrite(buzzerPin, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
}
