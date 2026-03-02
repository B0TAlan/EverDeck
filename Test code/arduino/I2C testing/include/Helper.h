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
    int get_accel(int index);
    int get_gyro(int index);
    int get_accel_raw(int index, int *data);
    int get_gyro_raw(int index, int *data);

    private:

    uint8_t rawData[14];
    uint8_t aX;
    uint8_t aY;
    uint8_t aZ;
    uint8_t gX;
    uint8_t gY;
    uint8_t gZ;
    const int addrs[2] = {0x68, 0x69};
    int addr;
    

};

#endif