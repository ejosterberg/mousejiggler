#include "DigiMouse.h"

  unsigned long previousMouseMillis=0;
  unsigned long previousUpdateMouseMillis=0;
  unsigned long previousLedMillis=0;

  int ledState = LOW;
  int mouseDirection = 1; 

  // 1000 = 1 second // Frequency of LED blink
  int long ledBlinkRate = 1000;
  // 3000 = 3.5 seconds // Hold LED on to warn of move about to come
  const long ledHoldRate = 3500;
  // 59000 = 59 Seconds // Frequency of mouse jitter
  const long mouseMoveRate = 59000;

  const int ledPin = 1;
     
void setup() {
  DigiMouse.begin(); //start or reenumerate USB - BREAKING CHANGE from old versions that didn't require this
  pinMode(ledPin, OUTPUT); //Define LED as an output
}

void loop() {

  // Grab the current time in Millis
  unsigned long currentMillis = millis();

  if (currentMillis - previousLedMillis >= ledBlinkRate) {

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
      // If the LED is on, make the function run quite a bit sooner so we turn off the LED
      ledBlinkRate = ledBlinkRate - 930;
    } else {
      ledState = LOW;
      // If the LED is off, we can wait a bit longer before we run again... This changes the duty cycle of the LED blink
      ledBlinkRate = ledBlinkRate + 930;
    }


    // Time to toggle the LED set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);

    // If we're about to move the mouse, immediately toggle the LED back on.
    if (currentMillis - previousMouseMillis  >= (mouseMoveRate - ledHoldRate)) {
      digitalWrite(ledPin, HIGH);
      } 

    // save the last time you blinked the LED
    previousLedMillis = currentMillis;
  }  

  // Lets check and see if we should move the mouse.
  if (currentMillis - previousMouseMillis >= mouseMoveRate) {
    // save the last time you moved the mouse
    previousMouseMillis = currentMillis;

    // if the mouse moved one way, move the other, then update the var so we know what direction next time
    if (mouseDirection == 1) {
      mouseDirection = 0;
      DigiMouse.move(-4,0,0,0); //Move it one way
    } else {
      mouseDirection = 1;
      DigiMouse.move(4,0,0,0); //Move it back the other way
    }
  }
  
  // If not using plentiful DigiMouse.delay(), make sure to call 
  // DigiMouse.update() at least every 50ms - I found we need to do this often or we upset Windows OS
  if (currentMillis - previousUpdateMouseMillis >= 6) {
    previousUpdateMouseMillis = currentMillis;
    DigiMouse.update();
  }
  
}
