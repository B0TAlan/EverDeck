#include<Helper.h>
#define addr1 0x01
#define addr2 0x02

#define MPU_ADDR 0x68   // default I2C address (AD0 = GND)

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



int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  Wire.begin(addr2);
  Serial.begin(9600);
  Wire.onRequest(r);
  Wire.onReceive(b);
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
  
  /*h.readWireFrom(addr2,2,msg);

  //h.sendWireTo(addr2, 2, d);

  

  //Serial.println(msg[0]);
  //Serial.println(msg[1]);

  uint8_t rawData[14];

  // Read starting at ACCEL_XOUT_H (0x3B), total 14 bytes
  mpuReadBytes(0x3B, rawData, 14);

  int16_t accelX = (rawData[0] << 8) | rawData[1];
  int16_t accelY = (rawData[2] << 8) | rawData[3];
  int16_t accelZ = (rawData[4] << 8) | rawData[5];

  int16_t tempRaw = (rawData[6] << 8) | rawData[7];

  int16_t gyroX  = (rawData[8] << 8) | rawData[9];
  int16_t gyroY  = (rawData[10] << 8) | rawData[11];
  int16_t gyroZ  = (rawData[12] << 8) | rawData[13];

  Serial.print("AX: "); Serial.print(accelX);
  Serial.print("\tAY: "); Serial.print(accelY);
  Serial.print("\tAZ: "); Serial.print(accelZ);

  Serial.print("\tGX: "); Serial.print(gyroX);
  Serial.print("\tGY: "); Serial.print(gyroY);
  Serial.print("\tGZ: "); Serial.print(gyroZ);

  Serial.print("\tTEMP_RAW: "); Serial.println(tempRaw);
  Serial.print("\ti2cA: "); Serial.println(msg[0]);
  Serial.print("\ti2cB: "); Serial.println(msg[1]);

  delay(100);*/

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