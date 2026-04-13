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

int Helper::debounce(int pin){
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

void MPU6050::WriteByte(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);
}

void MPU6050::ReadBytes(uint8_t reg, uint8_t *buf, uint8_t len) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission(false); // repeated start

  Wire.requestFrom(addr, len, true);

  for (uint8_t i = 0; i < len; i++) {
    if (Wire.available()) {
      buf[i] = Wire.read();
    }
  }
}

void MPU6050::init(){
  WriteByte(0x6B, 0x00);
}

int16_t *MPU6050::get_accel(){
  ReadBytes(0x3B, rawData, 14);

  accel[0] = (rawData[0] << 8) | rawData[1];
  accel[1] = (rawData[2] << 8) | rawData[3];
  accel[2] = (rawData[4] << 8) | rawData[5];

  return accel;
}

int16_t *MPU6050::get_gyro(){
  ReadBytes(0x3B, rawData, 14);
  
  gyro[0]  = (rawData[8] << 8) | rawData[9];
  gyro[1]  = (rawData[10] << 8) | rawData[11];
  gyro[2]  = (rawData[12] << 8) | rawData[13];

  return gyro;
}

uint8_t *MPU6050::get_raw(){
  ReadBytes(0x3B, rawData, 14);
  return rawData;
}