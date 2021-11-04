#include <Arduino.h>
#include <HCSR04.h>
#include <SevSeg.h>

static unsigned long timer = millis();
static unsigned long measureTimer = millis();
static unsigned long displayTimer = millis();

#define ECHO 4
#define TRIG 3

// #define digit1 14
// #define digit2 15
// #define digit3 16
// #define segA 5
// #define segB 6
// #define segC 7
// #define segD 8
// #define segE 9
// #define segF 10
// #define segG 11
// #define segDP 12

float distance = 0;
HCSR04 ultrasonicSensor(TRIG, ECHO, 24, 400);
SevSeg sevseg;

void blink(void){
  if (millis() - timer > 500) {
    timer = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void measure(void){
  if (millis() - measureTimer > 250) {
    measureTimer = millis();
    // distance = ultrasonicSensor.getMedianFilterDistance();
    distance = ultrasonicSensor.getDistance();
    if (distance != HCSR04_OUT_OF_RANGE){
      Serial.print(distance, 1);
      Serial.println(F(" cm"));
    }
    else{
      Serial.println(F("out of range"));
    }
  }
}

void display(void){

  if (millis() - displayTimer >= 100) {
    if (distance < 100){
      sevseg.setNumberF(distance, 1);
    }
    else{
      sevseg.setNumber(distance);
    }

  }
  sevseg.refreshDisplay(); // Must run repeatedly
}

void sevSegInit(void){
  byte numDigits = 3;
  byte digitPins[] = {14, 15, 16};
  byte segmentPins[] = {5, 6, 7, 8, 9, 10, 11, 12};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  ultrasonicSensor.begin();
  sevSegInit();
}

void loop() {
  blink();
  measure();
  display();
}
