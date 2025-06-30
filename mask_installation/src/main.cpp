#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Global variables:
const int ledPin = 1; 
const int switch_pin = 3;
volatile bool buttonPressed = false; 


// Function declarations:
void IRAM_ATTR isr();
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);


void setup() {
  Serial.begin(921600);
  Serial.println();
  
  // Initialize SD Card:
  Serial.println("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // Initialization button + relay:
  pinMode(ledPin, OUTPUT);
  pinMode(switch_pin, INPUT);
  buttonPressed = digitalRead(switch_pin);
  attachInterrupt(switch_pin, isr, CHANGE);
  
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
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

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}