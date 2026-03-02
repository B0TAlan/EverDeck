#include "Helper.h"

Helper::Helper() {}

void Helper::readWireFrom(int addr, int length, int *var)
{
    Wire.requestFrom(addr, length * 2);
    int i = 0;
    while (Wire.available())
    { // While there are bytes to read from the slave
        if (i < length)
        {                              // Ensure we don't write beyond our array bounds
            byte msb = Wire.read();    // Read the Most Significant Byte
            byte lsb = Wire.read();    // Read the Least Significant Byte
            var[i] = (msb << 8) | lsb; // Reconstruct the integer
            i++;
        }
        else
        {
            // If slave sends more than expected, just read and discard
            Wire.read();
        }
    }
}

void Helper::readWire(int *var)
{
    int i = 0;
    while (Wire.available())
    { // While there are bytes to read from the slave
        if (i < (Wire.available()/2))
        {                              // Ensure we don't write beyond our array bounds
            byte msb = Wire.read();    // Read the Most Significant Byte
            byte lsb = Wire.read();    // Read the Least Significant Byte
            var[i] = (msb << 8) | lsb; // Reconstruct the integer
            i++;
        }
        else
        {
            // If slave sends more than expected, just read and discard
            Wire.read();
        }
    }
}

void Helper::sendWire(int lenght, int* var)
{
    for(int i = 0; i<lenght;i++){
    Wire.write((byte)(var[i] >> 8));  // MSB
    Wire.write((byte)(var[i] & 0xFF)); // LSB
  }    
}

void Helper::sendWireTo(int addr, int lenght, int* var)
{ 
  Wire.beginTransmission(addr);
  for(int i = 0; i<lenght;i++){
    Wire.write((byte)(var[i] >> 8));  // MSB
    Wire.write((byte)(var[i] & 0xFF)); // LSB
  }
  Wire.endTransmission();    
}

int debouce(int pin){
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

MPU6050::MPU6050(int ADDR){
  addr = addrs[ADDR];
}