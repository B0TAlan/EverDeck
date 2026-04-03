#include<Helper.h>
#include"Sinput.h"
#define addr1 0x01
#define addr2 0x02

#define MPU_ADDR 0x68   // default I2C address (AD0 = GND)

MPU6050 mpu(0);


void r();
void b(int byte);

void mpuWriteByte(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);
}

void mpuReadBytes(uint8_t reg, uint8_t *buf, uint8_t len) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false); // repeated start

  Wire.requestFrom(MPU_ADDR, len, true);

  for (uint8_t i = 0; i < len; i++) {
    if (Wire.available()) {
      buf[i] = Wire.read();
    }
  }
}

SInputHID controller;
//const uint8_t BTN_PINS[] = { 3 };

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  //Wire.begin(addr2);
  Serial.begin(9600);
  controller.begin();
  //Wire.onRequest(r);
  //Wire.onReceive(b);
  pinMode(3,INPUT);
  //mpu.init();
 // Wake up MPU6050 (clear sleep bit)
  //mpuWriteByte(0x6B, 0x00);

  // Optional: set gyro range (±250 deg/s)
  //mpuWriteByte(0x1B, 0x00);

  // Optional: set accel range (±2g)
  //mpuWriteByte(0x1C, 0x00);

}

Helper h;

int* msg;
int* bb;
int d[] = {6, 6};
void loop() {

  
  
  //h.readWireFrom(addr2,2,msg);
  
  //h.sendWireTo(addr2, 2, d);
  uint8_t BTN_PINS[] = { 3 };
  controller.readButtonPins(BTN_PINS, 1);
  controller.sendReport();

  //Serial.println(msg[0]);
  //Serial.println(msg[1]);

  //uint8_t rawData[14];

  // Read starting at ACCEL_XOUT_H (0x3B), total 14 bytes
  //mpuReadBytes(0x3B, rawData, 14);

  //int16_t accelX = mpu.get_accel()[0];
  //int16_t accelY = mpu.get_accel()[1];
  //int16_t accelZ = mpu.get_accel()[2];
//
  //int16_t gyroX  = mpu.get_gyro()[0];
  //int16_t gyroY  = mpu.get_gyro()[1];
  //int16_t gyroZ  = mpu.get_gyro()[2];
//
  Serial.println(h.debounce(3));
  //Serial.print("\tAY: "); Serial.print(accelY);
  //Serial.print("\tAZ: "); Serial.print(accelZ);

  //Serial.print("\tGX: "); Serial.print(gyroX);
  //Serial.print("\tGY: "); Serial.print(gyroY);
  //Serial.print("\tGZ: "); Serial.print(gyroZ);

  delay(100);

}

void r(){
  int m[] = {1,3};
  h.sendWire(2,m);
}

void b(int byte){
  h.readWire(bb);
  Serial.println(byte);
  for(int i =0; i<byte; i++){
    Serial.print(bb[i]);
    Serial.print("  ");
  }
  Serial.println();

}