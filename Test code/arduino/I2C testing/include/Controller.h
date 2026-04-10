#ifndef CONTROLLER_H
#define CONTROLLER_H
#include<Arduino.h>
#include<Sinput.h>


//uint8_t button_pins[] = {7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

namespace ControllerButtons {// this struct contains a bitmap for each button to avoid adding padding bits
    static constexpr uint8_t A        = 0; //Btn0
    static constexpr uint8_t B        = 1; //Btn1
    static constexpr uint8_t X        = 4; //Btn2
    static constexpr uint8_t Y        = 3; //Btn3
    static constexpr uint8_t LB       = 6; //Btn4
    static constexpr uint8_t RB       = 7; //Btn5
    static constexpr uint8_t BACK     = 10; //Btn6
    static constexpr uint8_t START    = 11; //Btn7
    static constexpr uint8_t L3       = 13; //Btn8
    static constexpr uint8_t R3       = 14; //Btn9
    static constexpr uint8_t MENU     = 12; //Btn10
    static constexpr uint8_t PAD1     = 15; //Btn11
    static constexpr uint8_t PAD2     = 16; //Btn12
    static constexpr uint8_t PAD3     = 17; //Btn13

    static constexpr uint8_t Masks[] = {A, B, X, Y, LB, RB, BACK, START, L3, R3, MENU, PAD1, PAD2, PAD3 };
};
//int xOut;
//int yOut;
//int r2;
//int l2;

class Controller{
    public:
    Controller(uint8_t* pins, size_t pinNum, uint16_t* ls, uint16_t* rs, uint16_t* trig);
    Controller(uint8_t* pins, size_t pinNum, uint16_t* ls, uint16_t* rs, uint16_t* trig, int* dz);
    
    void setBut(uint8_t Button, int value);

    void bindBut(uint8_t Button, int pin);

    void send();

    void begin();

    void stop();

    void start();

    void printButtonDebug();

    private:
    //vars
    int *deadZone; // {ls,rs,lt,rt}
    bool STATE = true;
    uint16_t *lStick, *rStick, *triggers; // analog stick {x, y} & triggers {L, R}
    uint8_t *buttons, *Dpad; 
    Gamepad_Input_Report_t  input  = {};  // zero-initialised on startup
    Gamepad_Output_Report_t output = {};
    size_t bn;

    //funcs
    uint8_t *readBut(uint8_t *pins);
    uint8_t deb(uint8_t pin);

    template <typename T>
static int get_index(const T* array, size_t size, T target) {
    for (size_t i = 0; i < size; i++) {
        if (array[i] == target) return (int)i;
    }
    return -1;
}

    

};

#endif