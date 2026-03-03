#ifndef HELPER_H
#define HELPER_H

#include <Arduino.h>
#include <Wire.h>


class Helper
{
public:

    Helper();

    void readWire(int* var);
    void readWireFrom(int addr, int length, int* var);
    void sendWire(int lenght, int* var);
    void sendWireTo(int addr, int lenght, int* var);
};

int debounce(int pin);

class MPU6050 {
    public:
    
    MPU6050(int ADDR);

    void init();
    void WriteByte(uint8_t reg, uint8_t data);
    void ReadBytes(uint8_t reg, uint8_t *buf, uint8_t len);
    void Reset();
    void Start();
    int16_t *get_accel();
    int16_t *get_gyro();
    uint8_t *get_raw();
    

    private:

    uint8_t rawData[14];
    int16_t accel[3];
    int16_t gyro[3];
    const int addrs[2] = {0x68, 0x69};
    int addr;
    

};

#endif