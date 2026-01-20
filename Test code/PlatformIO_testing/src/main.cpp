#include <Arduino.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "Joystick.h"
#include <Wire.h>
// put function declarations here:

int mode = 0;

void bindK(int var);
void bindM(int var);
void bindJ(int var);
int debounce(int pin);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  1, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){}
  pinMode(8, INPUT);
  pinMode(9,INPUT);
  Mouse.begin();
  Keyboard.begin();
  Joystick.begin();

  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (debounce(8) == HIGH){
    mode++;
    delay(20);
  }
  if (mode > 2) {
    mode = 0;
  }

  Serial.print("mode: ");
  Serial.println(mode);

  //Serial.print("pin 9: ");
 // Serial.println(debounce(9));

  switch (mode)
  {
  case 1:
    bindK(debounce(9));
    break;
  case 2:
    bindM(debounce(9));
    break;
  default:
    bindJ(debounce(9));
    break;
  }

  //delay(100);
}

int debounce(int pin){
  int buttonState; 
  int lastButtonState = 0;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 10; 
  int raw = digitalRead(pin);
  
  if (raw != lastButtonState){
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:a

    // if the button state has changed:
    if (raw != buttonState) {
      buttonState = raw;

    }

    if (buttonState > 0){
      buttonState = HIGH;
    }
  }

  return buttonState;
}

void bindK(int var){
  if(var == 1){
    Keyboard.write('a');
  }

}

void bindM(int var){
  if(var == 1){
    Mouse.click(MOUSE_RIGHT);

  }

}

void bindJ(int var){
  if(var == 1){
    Joystick.pressButton(0);
  }
  else{
    Joystick.releaseButton(0);
  }
}