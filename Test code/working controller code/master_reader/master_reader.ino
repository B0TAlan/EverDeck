

// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <XInput.h>

int getB(int addr, int length, int* var){
  Wire.requestFrom(addr, length * 2);
  int i = 0;
 while (Wire.available()) { // While there are bytes to read from the slave
    if (i < length) { // Ensure we don't write beyond our array bounds
      byte msb = Wire.read(); // Read the Most Significant Byte
      byte lsb = Wire.read(); // Read the Least Significant Byte
      var[i] = (msb << 8) | lsb; // Reconstruct the integer
      i++;
    } else {
      // If slave sends more than expected, just read and discard
      Wire.read();
    }
  }
}

int getIndex(int key, int len, int* array){
  

  for (int i = 0; i < len; i++){
    if (array[i] == key){
      return i;
    }
  }
  return -1;
}

int joySmooth(int raw, int dz, int mid){
  int up = mid + dz;
  int dow = mid - dz;
  if(raw >= up || raw <= dow){
    return raw;
  }
  return mid;
}

void setup() {
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  Wire.begin();        // join I2C bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  //Joystick.begin();
  XInput.begin();
  XInput.setJoystickRange(0, 1022);
  //Keyboard.begin();
}

void loop() {
  //int B = getB(8,1);
  //Serial.println(B);
   delay(50); // Wait a couple of seconds before requesting data

  int xValue = analogRead(A1);
  int yValue = analogRead(A2);

  //Serial.println(xValue);
  //Serial.println(yValue);
 
  int hat[4] = {digitalRead(4), digitalRead(5), digitalRead(6), digitalRead(7)};
  int h = getIndex(0,4, hat);

  //Serial.println(h);
     
  //Serial.println(h);
  //Joystick.setHatSwitch(1, 180);
  //Joystick.setHatSwitch(2, (digitalRead(6)*-1));
  // Request ARRAY_SIZE * 2 bytes (2 bytes per int) from the slave
  // The slave will respond with the array when this request is made.
  Wire.requestFrom(8, 8);

  int receivedArray[9];
  int i = 6;

  int t = getB(8,9,receivedArray);

  XInput.setButton(0,!digitalRead(9));
  XInput.setButton(7,!digitalRead(8));
  XInput.setButton(8, !receivedArray[4]);

  // face but
  XInput.setButton(1, !receivedArray[0]);
  XInput.setButton(2, !receivedArray[1]);
  XInput.setButton(3, !receivedArray[3]);
  XInput.setButton(4, !receivedArray[2]);
  
  // D-Pad
  XInput.setButton(11,!hat[2]);
  XInput.setButton(12,!hat[0]);
  XInput.setButton(13,!hat[1]); 
  XInput.setButton(14,!hat[3]);


  XInput.setJoystickX(JOY_RIGHT,joySmooth(receivedArray[7],3, 512),false);
  XInput.setJoystickY(JOY_RIGHT,joySmooth(receivedArray[8],3, 526),false);
  XInput.setJoystickX(JOY_LEFT,joySmooth(xValue,3, 512),false);
  XInput.setJoystickY(JOY_LEFT,joySmooth(yValue,3, 512),true);
 
  for (int a = 0; a < i; a++){
    //XInput.setButton(a, !receivedArray[a]);
  }
  if (i > 0) { // Only print if we received some data
  
    for (int j = 0; j < i; j++) {
     //Serial.print(hat[j]);
     //Serial.print(" ");
   }
  //Serial.println();
  } else {
   Serial.println("No data received from slave.");
  }
}

 