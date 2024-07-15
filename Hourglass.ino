const int ledPins[] = {9, 10, 11, 4, 5, 6};  // LED pins
const int tiltPin = 2;  // Tilt switch pin
const int numLeds = 6;  // Total number of LEDs
const long interval = 10000;  // 10 seconds in milliseconds

unsigned long previousMillis = 0;
int ledState = 0;
bool isRunning = false;
bool testMode = true;  // Set to true for LED test, false for hourglass mode

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  Serial.println("Starting setup...");

  // Set LED pins as outputs
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);  // Ensure all LEDs start off
  }
  
  // Set tilt switch pin as input with pull-up resistor
  pinMode(tiltPin, INPUT_PULLUP);
  
  // Attach interrupt for tilt switch
  attachInterrupt(digitalPinToInterrupt(tiltPin), resetHourglass, FALLING);

  Serial.println("Setup complete. Starting main loop.");
}

void loop() {
  if (testMode) {
    ledTest();
  } else {
    hourglassMode();
  }
}

void ledTest() {
  for (int i = 0; i < numLeds; i++) {
    Serial.print("Testing LED on pin ");
    Serial.println(ledPins[i]);
    digitalWrite(ledPins[i], HIGH);
    delay(1000);
    digitalWrite(ledPins[i], LOW);
    delay(500);
  }
}

void hourglassMode() {
  unsigned long currentMillis = millis();

  if (isRunning) {
    Serial.println("Hourglass is running");
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      
      if (ledState < numLeds) {
        digitalWrite(ledPins[ledState], HIGH);
        Serial.print("Turning on LED ");
        Serial.println(ledState);
        ledState++;
      } else {
        isRunning = false;
        Serial.println("All LEDs lit, stopping");
      }
    }
  } else {
    Serial.println("Hourglass is not running");
  }
}

void resetHourglass() {
  Serial.println("Resetting hourglass");
  // Reset all LEDs
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  
  ledState = 0;
  isRunning = true;
  previousMillis = millis();
}