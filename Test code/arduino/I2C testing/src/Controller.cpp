#include"Controller.h"

Controller::Controller(uint8_t* pins,size_t pinNum, uint16_t* ls, uint16_t* rs, uint16_t* trig, bool buttype){
    buttons = pins;
    lStick = ls;
    triggers = trig;
    butType = buttype;
    bn = pinNum;
}

Controller::Controller(uint8_t* pins,size_t pinNum, uint16_t* ls, uint16_t* rs, uint16_t* trig){
    Controller( pins, pinNum,  ls, rs, trig, ButtonType::PIN);
}

Controller::Controller(uint8_t* pins, size_t pinNum, uint16_t* ls, uint16_t* rs, uint16_t* trig, int* dz, bool buttype){
    Controller(pins, pinNum, ls, rs, trig, buttype);
    deadZone = dz;
}

Controller::Controller(uint8_t* pins, size_t pinNum, uint16_t* ls, uint16_t* rs, uint16_t* trig, int* dz){
    Controller( pins,  pinNum,  ls, rs, trig, dz, ButtonType::PIN);
}

Controller::Controller(bool bt){
    uint8_t ttt[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    butType = bt;
    buttons = ttt;
}

void Controller::begin(){
    static HIDSubDescriptor node(
            sinput_hid_report_descriptor,
            sizeof(sinput_hid_report_descriptor)
        );
        
        HID().AppendDescriptor(&node);
}

void Controller::start(){
    STATE = true;
}

void Controller::stop(){
    STATE = false;
}

void Controller::send(){
    
    for (uint8_t i = 0; i<bn; i++){
        if (butType == true) setBut(ControllerButtons::Masks[i], deb(buttons[i]));
        else setBut(ControllerButtons::Masks[i], buttons[i]);

        //Serial.print(buttons[i]);
        Serial.print(i);
    }
    
    if (STATE == true) HID().SendReport(1, &input, sizeof(input));


}

void Controller::setBut(uint8_t Button, int value){
 if (Button >= 32) return; // Safety: don't crash the Pro Micro

    uint8_t byteIdx = Button / 8; // Integer division: 0-7=0, 8-15=1, etc.
    uint8_t bitIdx  = Button % 8; // Remainder: 0-7

    if (value >= 1 && value != 86) { // If pressed (HIGH)
        input.buttons[byteIdx] |= (1 << bitIdx);
    
    }
    else { // If released (LOW)
        input.buttons[byteIdx] &= ~(1 << bitIdx);
    }
    
}


void Controller::printButtonDebug() {
    Serial.print("Buttons: ");
    for (int i = 0; i < 4; i++) {
        // Access the array element directly
        uint8_t byteVal = input.buttons[i]; 

        Serial.print("Byte ");
        Serial.print(i);
        Serial.print(": ");
        
        for (int b = 7; b >= 0; b--) {
            Serial.print(bitRead(byteVal, b));
        }
        
        if (i < 3) Serial.print(" | ");
    }
    Serial.println();
}

void Controller::bindBut(uint8_t Button, int pin){
    int index = get_index(ControllerButtons::Masks, 14, Button);
    buttons[index] = pin;
}

void Controller::butNum(size_t num){
    bn = num;
}

uint8_t Controller::deb(uint8_t pin){
  int buttonState; 
  int lastButtonState = 0;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 10; 
  int raw = digitalRead(pin);
  //Serial.println(raw);
  //Serial.println(millis());
  if (pin == 86){
    return 0;
  }
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