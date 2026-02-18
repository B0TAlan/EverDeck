// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int test[9]; 

void setup() {
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  Serial.begin(4800);
  Wire.begin(8);                // join I2C bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  test[0] = digitalRead(6);
  test[1] = digitalRead(5);
  test[2] = digitalRead(4);
  test[3] = digitalRead(7); 
  test[4] = digitalRead(8);
  test[5] = digitalRead(9);
  test[6] = digitalRead(10);

  int xValue = analogRead(A1);
  int yValue = analogRead(A2);

  test[7] = xValue;
  test[8]= yValue;

  //Serial.println(xValue);
  //Serial.println(yValue);

  delay(50);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  for(int i = 0; i<9;i++){
    Wire.write((byte)(test[i] >> 8));  // MSB
    Wire.write((byte)(test[i] & 0xFF)); // LSB
  }
}
