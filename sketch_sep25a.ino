#include <Key.h>
#include <Keypad.h>

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'} // 'D' is reset
};
byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pins for LEDs and buzzer
const int greenLED = 10;
const int redLED = 11;
const int buzzer = 12;

// Passcode setup
String passcode = "1234";
String input = "";

// Timing
unsigned long startTime;
const unsigned long timeout = 7000; // 7 seconds

void setup() {
  Serial.begin(9600);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  char key = keypad.getKey();

  // Reset check
  if(key == 'D') {
    input = "";
    Serial.println("Input reset!");
    return; // skip the rest of loop
  }

  if(key) { // Key pressed
    input += key;
    Serial.print("Key pressed: ");
    Serial.println(key);

    // Start timeout timer on first key press
    if(input.length() == 1) startTime = millis();
  }

  // Check timeout
  if(input.length() > 0 && millis() - startTime > timeout) {
    Serial.println("Timeout!");
    // TODO: play timeout error tone
    input = "";
  }

  // Check passcode if input length matches
  if(input.length() == passcode.length()) {
    if(input == passcode) {
      Serial.println("Correct!");
      digitalWrite(greenLED, HIGH);
      happyMelody();
      delay(2000);
      digitalWrite(greenLED, LOW);
    } else {
      Serial.println("Incorrect!");
      digitalWrite(redLED, HIGH);
      errorTone();
      delay(2000);
      digitalWrite(redLED, LOW);
    }
    input = ""; // reset for next attempt
  }
}

void happyMelody() {
  tone(buzzer, 523, 200); //C5
  delay(250);
  tone(buzzer, 659, 200); //E5
  delay(250);
  tone(buzzer, 784, 200); //G5
  delay(350);
  noTone(buzzer);
}

void errorTone() {
  tone(buzzer, 200, 500); //C5
  delay(500);
  noTone(buzzer);
}
void songOfStorms() {
  int melody[] = {293, 440, 587, 293, 440, 587, 659, 698, 659, 698, 659, 523, 440, 440, 293, 349, 392, 440, 440, 293, 349, 392, 329, 293, 440, 587, 293, 440, 587, 659, 698, 659, 698, 659, 523, 440, 440, 293, 349, 392, 440, 440, 293}; // notes (Hz)
  int duration[] = {125, 125, 500, 125, 125, 500, 375, 85, 85, 85, 125, 125, 500, 250, 250, 125, 125, 750, 250, 250, 125, 125, 750, 125, 125, 500, 125, 125, 500, 375, 85, 85, 85, 125, 125, 500, 250, 250, 125, 125, 500, 250, 1500}; // duration (ms)

  for (int i = 0; i < 43; i++) {
    tone(buzzer, melody[i], duration[i]);
    delay(duration[i] + 50); // short pause between notes
  }
  noTone(buzzer);
}
