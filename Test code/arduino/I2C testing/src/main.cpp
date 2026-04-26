#include<Helper.h>
#include"Sinput.h"
#include"Controller.h"
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

uint8_t deb(uint8_t pin){
  int buttonState; 
  int lastButtonState = 0;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 10; 
  int raw = digitalRead(pin);
  //Serial.println(raw);
  //Serial.println(millis());
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

uint8_t msg[4];

uint8_t ttt[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t BTN_PINS[] = { 4, 5, 6, 9, 86, 86, 10, 16, 86, 86, 86, 86, 86, 86, 86, 86, 86};
uint16_t holder[] = {0,0};
SInputHID Scontroller;
Controller cont(false);
//const uint8_t BTN_PINS[] = { 3 };

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  Wire.begin(addr1);
  Serial.begin(9600);
  //Scontroller.begin();
  cont.butNum(15);
  cont.begin();
  //Wire.onRequest(r);
  //Wire.onReceive(b);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(9,INPUT);
  pinMode(16,INPUT);
  pinMode(10,INPUT);
  //mpu.init();
 // Wake up MPU6050 (clear sleep bit)
  //mpuWriteByte(0x6B, 0x00);

  // Optional: set gyro range (±250 deg/s)
  //mpuWriteByte(0x1B, 0x00);

  // Optional: set accel range (±2g)
  //mpuWriteByte(0x1C, 0x00);

}
int i = 0;
Helper h;


int* bb;
int d[] = {6, 6};
void loop() {

  h.readu(addr2,4,msg);
  uint8_t bb[] = {cont.deb(4), cont.deb(5), cont.deb(6),cont.deb(9), 0, 0, cont.deb(10), cont.deb(16), 0, 0, 0, msg[0], msg[1], msg[2], msg[3]};
  cont.buttons = bb;
  //cont.bindBut(ControllerButtons::A, cont.deb(4));
  //cont.bindBut(ControllerButtons::B, cont.deb(5));
  //cont.bindBut(ControllerButtons::X, cont.deb(6));
  //cont.bindBut(ControllerButtons::Y, cont.deb(9));
  //cont.bindBut(ControllerButtons::START, cont.deb(10));
  //cont.bindBut(ControllerButtons::BACK, cont.deb(16));
  //cont.bindBut(ControllerButtons::UP, msg[0]);
  //cont.bindBut(ControllerButtons::DWN, msg[1]);
  //cont.bindBut(ControllerButtons::LEFT, msg[2]);
  //cont.bindBut(ControllerButtons::RIGHT, msg[3]);
  cont.send();
  Serial.println(cont.deb(10));
  //Serial.print(msg[1]);
  //Serial.print(msg[2]);
  //Serial.println(msg[3]);
  //h.sendWireTo(addr2, 2, d);
  
  
  //cont.printButtonDebug();
  //Scontroller.readButtonPins(BTN_PINS, 32);
  //Scontroller.setButton(3, true);
  //Scontroller.sendReport();

  
  //Serial.println(deb(9));
  //Serial.println(i);
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
  //Serial.println(deb(16));
  //Serial.println(deb(4));
  //Serial.println(deb(5));
  //Serial.println(deb(6));
  //Serial.println(deb(9));
  //Serial.println(digitalRead(3));
  //Serial.print("\tAY: "); Serial.print(accelY);
  //Serial.print("\tAZ: "); Serial.print(accelZ);

  //Serial.print("\tGX: "); Serial.print(gyroX);
  //Serial.print("\tGY: "); Serial.print(gyroY);
  //Serial.print("\tGZ: "); Serial.print(gyroZ);

  delay(10);

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