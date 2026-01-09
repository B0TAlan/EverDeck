// Simple arcade stick example that demonstrates how to read twelve
// Arduino Pro Micro digital pins and map them to the
// Arduino Joystick library.
//

// The digital pins 2 - 20 are grounded when they are pressed.
// Pin 10, A10, Red = UP
// Pin 15, D15, Yellow = RIGHT
// Pin 16, D16, Orange = DOWN
// Pin 14, D14, Green = LEFT

// Pin 9, A9 = Button 1
// Pin 8, A8 = Button 2
// Pin 7, D7 = Button 3
// Pin 3, D3 = Button 4
// Pin 2, D2 = Button 5
// Pin 4, A6 = Button 6

// Pin 20, A2 = Select Button 1
// Pin 19, A1 = Start Button 2

// Pin 5, D5 = Other Button
// Pin 6, A7 = Other Button
// Pin 18, A0 = Other Button
// Pin 21, A3 = Other Button

// NOTE: This sketch file is for use with Arduino Pro Micro only.
//
// Original gamepad example by Matthew Heironimus
// 2016-11-24
// Adapted for arcade machine setup by Ben Parmeter
// 2019-05-20
//--------------------------------------------------------------------

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   8, 0,                  // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   true, true,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

void setup() {

  // Initialize Joystick Library
  pinMode(9, INPUT);
  Serial.begin(9600);
  //Joystick.begin();

  Joystick.setXAxisRange(95, 550);
  Joystick.setYAxisRange(550, 95);

  Joystick.setRudderRange(95, 550);
  Joystick.setThrottleRange(550, 95);
}

void loop() {
  //Serial.println(digitalRead(9));
  Serial.println(analogRead(A0));
  Serial.println(analogRead(A1));
  
  //Joystick.setRudder(analogRead(A0));
  //Joystick.setThrottle(analogRead(A1));

  delay(100);
}
