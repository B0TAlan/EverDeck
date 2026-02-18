#ifndef HELPER_H
#define HELPER_H

#include <Arduino.h>
#include <Wire.h>


class Helper
{
public:

    Helper();

    void readWire(int addr, int length, int* var);

    void sendWire(int lenght, int* var);
};

int debounce(int pin);

#endif