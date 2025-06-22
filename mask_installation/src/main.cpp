#include <Arduino.h>

// Global variables:
const int ledPin = 1; 
const int switch_pin = 3;
volatile bool buttonPressed = false; 

// Function declarations:
void IRAM_ATTR isr();

void setup() {

  Serial.begin(921600);
  pinMode(ledPin, OUTPUT);
  pinMode(switch_pin, INPUT);
  buttonPressed = digitalRead(switch_pin);
  attachInterrupt(switch_pin, isr, CHANGE);
}

void loop() {
  
  if (buttonPressed) {
    digitalWrite(ledPin, HIGH); 
  } else {
    digitalWrite(ledPin, LOW);
  }
  delay(200);
  
}

// Function definition
void IRAM_ATTR isr(){
  buttonPressed = digitalRead(switch_pin); 
}