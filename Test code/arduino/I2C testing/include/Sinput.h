#pragma once
#include <Arduino.h>
#include <HID-Project.h>


// =============================================================
//  sinput_hid.h
//  SInput-compatible HID descriptor + structs + helper class
//  For use with PlatformIO + Arduino framework + HID-Project
//
//  Usage:
//    1. Drop this file in your src/ folder
//    2. #include "sinput_hid.h" in main.cpp
//    3. Add  SInputHID controller;  in main.cpp
//    4. Call controller.begin() in setup()
//    5. Fill report fields and call controller.sendReport() in loop()
//
//  platformio.ini lib_deps:
//    NicoHood/HID-Project @ ^2.8.2
// =============================================================

// -------------------------------------------------------------
//  INPUT REPORT BYTE MAP  (Report ID 0x01, 40 bytes payload)
//  ┌─────────┬───────────────────────────────────────────────┐
//  │ Bytes   │ Field                                         │
//  ├─────────┼───────────────────────────────────────────────┤
//  │  0– 3   │ buttons        uint32  (32 digital buttons)   │
//  │  4– 5   │ lx             int16   (left stick X)         │
//  │  6– 7   │ ly             int16   (left stick Y)         │
//  │  8– 9   │ rx             int16   (right stick X)        │
//  │ 10–11   │ ry             int16   (right stick Y)        │
//  │ 12–13   │ ltrigger       uint16  (left analog trigger)  │
//  │ 14–15   │ rtrigger       uint16  (right analog trigger) │
//  │ 16–17   │ accel[0]       int16   (accel X)              │
//  │ 18–19   │ accel[1]       int16   (accel Y)              │
//  │ 20–21   │ accel[2]       int16   (accel Z)              │
//  │ 22–23   │ gyro[0]        int16   (gyro X)               │
//  │ 24–25   │ gyro[1]        int16   (gyro Y)               │
//  │ 26–27   │ gyro[2]        int16   (gyro Z)               │
//  │ 28      │ t1_touch       uint8   (trackpad 1 flags)     │
//  │ 29      │ t1_id          uint8   (trackpad 1 touch ID)  │
//  │ 30–31   │ t1_x           uint16  (trackpad 1 X, 0-1920) │
//  │ 32–33   │ t1_y           uint16  (trackpad 1 Y, 0-940)  │
//  │ 34      │ t2_touch       uint8   (trackpad 2 flags)     │
//  │ 35      │ t2_id          uint8   (trackpad 2 touch ID)  │
//  │ 36–37   │ t2_x           uint16  (trackpad 2 X, 0-1920) │
//  │ 38–39   │ t2_y           uint16  (trackpad 2 Y, 0-940)  │
//  └─────────┴───────────────────────────────────────────────┘
//
//  OUTPUT REPORT BYTE MAP  (Report ID 0x03, 5 bytes payload)
//  ┌─────────┬───────────────────────────────────────────────┐
//  │ Byte 0  │ rumble_left    uint8   (0-255)                │
//  │ Byte 1  │ rumble_right   uint8   (0-255)                │
//  │ Byte 2  │ led_r          uint8                          │
//  │ Byte 3  │ led_g          uint8                          │
//  │ Byte 4  │ led_b          uint8                          │
//  └─────────┴───────────────────────────────────────────────┘
// -------------------------------------------------------------

static const uint8_t sinput_hid_report_descriptor[] PROGMEM = {

    0x05, 0x01,             // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,             // USAGE (Gamepad)
    0xA1, 0x01,             // COLLECTION (Application)

    // ── Report ID 1 : Input (controller → PC) ──────────────
    0x85, 0x01,             //   REPORT_ID (1)

    // 32 buttons — 4 bytes
    0x05, 0x09,             //   USAGE_PAGE (Button)
    0x19, 0x01,             //   USAGE_MINIMUM (1)
    0x29, 0x20,             //   USAGE_MAXIMUM (32)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x25, 0x01,             //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,             //   REPORT_SIZE (1 bit)
    0x95, 0x20,             //   REPORT_COUNT (32)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Left stick X/Y — 4 bytes (signed 16-bit)
    0x05, 0x01,             //   USAGE_PAGE (Generic Desktop)
    0x09, 0x30,             //   USAGE (X)
    0x09, 0x31,             //   USAGE (Y)
    0x16, 0x00, 0x80,       //   LOGICAL_MINIMUM (-32768)
    0x26, 0xFF, 0x7F,       //   LOGICAL_MAXIMUM (32767)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x02,             //   REPORT_COUNT (2)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Right stick X/Y — 4 bytes (signed 16-bit)
    0x09, 0x32,             //   USAGE (Z)
    0x09, 0x35,             //   USAGE (Rz)
    0x16, 0x00, 0x80,       //   LOGICAL_MINIMUM (-32768)
    0x26, 0xFF, 0x7F,       //   LOGICAL_MAXIMUM (32767)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x02,             //   REPORT_COUNT (2)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Analog triggers L/R — 4 bytes (unsigned 16-bit)
    0x09, 0x33,             //   USAGE (Rx)
    0x09, 0x34,             //   USAGE (Ry)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x7F,       //   LOGICAL_MAXIMUM (32767)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x02,             //   REPORT_COUNT (2)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Accelerometer XYZ — 6 bytes (signed 16-bit, vendor page)
    0x06, 0x00, 0xFF,       //   USAGE_PAGE (Vendor 0xFF00)
    0x09, 0x20,             //   USAGE (accel X)
    0x09, 0x21,             //   USAGE (accel Y)
    0x09, 0x22,             //   USAGE (accel Z)
    0x16, 0x00, 0x80,       //   LOGICAL_MINIMUM (-32768)
    0x26, 0xFF, 0x7F,       //   LOGICAL_MAXIMUM (32767)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x03,             //   REPORT_COUNT (3)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Gyroscope XYZ — 6 bytes (signed 16-bit, vendor page)
    0x09, 0x23,             //   USAGE (gyro X)
    0x09, 0x24,             //   USAGE (gyro Y)
    0x09, 0x25,             //   USAGE (gyro Z)
    0x16, 0x00, 0x80,       //   LOGICAL_MINIMUM (-32768)
    0x26, 0xFF, 0x7F,       //   LOGICAL_MAXIMUM (32767)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x03,             //   REPORT_COUNT (3)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Trackpad 1 touch flags + ID — 2 bytes (uint8 each)
    0x09, 0x40,             //   USAGE (t1 touch flags)
    0x09, 0x41,             //   USAGE (t1 touch ID)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00,       //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,             //   REPORT_SIZE (8 bits)
    0x95, 0x02,             //   REPORT_COUNT (2)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Trackpad 1 X — 2 bytes (0-1920)
    0x09, 0x42,             //   USAGE (t1 X)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0x80, 0x07,       //   LOGICAL_MAXIMUM (1920)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x01,             //   REPORT_COUNT (1)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Trackpad 1 Y — 2 bytes (0-940)
    0x09, 0x43,             //   USAGE (t1 Y)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0xAC, 0x03,       //   LOGICAL_MAXIMUM (940)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x01,             //   REPORT_COUNT (1)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Trackpad 2 touch flags + ID — 2 bytes (uint8 each)
    0x09, 0x44,             //   USAGE (t2 touch flags)
    0x09, 0x45,             //   USAGE (t2 touch ID)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00,       //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,             //   REPORT_SIZE (8 bits)
    0x95, 0x02,             //   REPORT_COUNT (2)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Trackpad 2 X — 2 bytes (0-1920)
    0x09, 0x46,             //   USAGE (t2 X)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0x80, 0x07,       //   LOGICAL_MAXIMUM (1920)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x01,             //   REPORT_COUNT (1)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // Trackpad 2 Y — 2 bytes (0-940)
    0x09, 0x47,             //   USAGE (t2 Y)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0xAC, 0x03,       //   LOGICAL_MAXIMUM (940)
    0x75, 0x10,             //   REPORT_SIZE (16 bits)
    0x95, 0x01,             //   REPORT_COUNT (1)
    0x81, 0x02,             //   INPUT (Data, Variable, Absolute)

    // ── Report ID 3 : Output (PC → controller) ─────────────
    // Rumble motors + RGB LED — 5 bytes
    0x85, 0x03,             //   REPORT_ID (3)
    0x09, 0x50,             //   USAGE (rumble left)
    0x09, 0x51,             //   USAGE (rumble right)
    0x09, 0x52,             //   USAGE (LED R)
    0x09, 0x53,             //   USAGE (LED G)
    0x09, 0x54,             //   USAGE (LED B)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x00,       //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,             //   REPORT_SIZE (8 bits)
    0x95, 0x05,             //   REPORT_COUNT (5)
    0x91, 0x02,             //   OUTPUT (Data, Variable, Absolute)

    0xC0                    // END_COLLECTION
};

// -------------------------------------------------------------
//  Structs — byte layout must match descriptor exactly
// -------------------------------------------------------------

typedef struct {
    uint32_t buttons;       // 32 digital buttons, 1 bit each
    int16_t  lx, ly;        // left stick  (-32768 to 32767)
    int16_t  rx, ry;        // right stick (-32768 to 32767)
    uint16_t ltrigger;      // left trigger  (0 to 32767)
    uint16_t rtrigger;      // right trigger (0 to 32767)
    int16_t  accel[3];      // accelerometer X, Y, Z
    int16_t  gyro[3];       // gyroscope X, Y, Z
    uint8_t  t1_touch;      // trackpad 1: bit0=f1 active, bit1=f2 active
    uint8_t  t1_id;         // trackpad 1: touch ID (increments each contact)
    uint16_t t1_x;          // trackpad 1: X position (0-1920)
    uint16_t t1_y;          // trackpad 1: Y position (0-940)
    uint8_t  t2_touch;      // trackpad 2: touch flags
    uint8_t  t2_id;         // trackpad 2: touch ID
    uint16_t t2_x;          // trackpad 2: X position (0-1920)
    uint16_t t2_y;          // trackpad 2: Y position (0-940)
} __attribute__((packed)) Gamepad_Input_Report_t;

typedef struct {
    uint8_t report_id;      // will be 0x03 when received
    uint8_t rumble_left;    // 0-255
    uint8_t rumble_right;   // 0-255
    uint8_t led_r;
    uint8_t led_g;
    uint8_t led_b;
} __attribute__((packed)) Gamepad_Output_Report_t;

// Compile-time check — fires if struct and descriptor get out of sync
static_assert(sizeof(Gamepad_Input_Report_t) == 40,
    "Input report struct size mismatch — check descriptor vs struct");

// -------------------------------------------------------------
//  SInputHID class
// -------------------------------------------------------------


class SInputHID {
public:

    uint8_t _rawBuf[64];
    // Call once in setup() before anything else
    void begin() {
        static HIDSubDescriptor node(
            sinput_hid_report_descriptor,
            sizeof(sinput_hid_report_descriptor)
        );
        RawHID.begin(_rawBuf, sizeof(_rawBuf));  // add this line
        HID().AppendDescriptor(&node);
    }

    // Call at the end of loop() after filling report fields
    void sendReport() {
        HID().SendReport(1, &input, sizeof(input));
    }

    // Call in loop() to check for incoming rumble/LED commands

    // ── Buttons ───────────────────────────────────────────────

    // Set a single button by number (1-32)
    // pressed=true sets the bit, pressed=false clears it
    void setButton(uint8_t num, bool pressed) {
        if (num < 1 || num > 32) return;
        uint8_t bit = num - 1;
        if (pressed) input.buttons |=  (1UL << bit);
        else         input.buttons &= ~(1UL << bit);
    }

    // Read an array of pin numbers and pack into the buttons field
    // pins[0] → button 1 (bit 0), pins[1] → button 2 (bit 1), etc.
    // Pins are assumed INPUT_PULLUP: LOW = pressed
    void readButtonPins(const uint8_t* pins, uint8_t count) {
        input.buttons = 0;
        for (uint8_t i = 0; i < count && i < 32; i++) {
            if (digitalRead(pins[i]) == HIGH)
                input.buttons |= (1UL << i);
        }
    }

    // ── Analog sticks ─────────────────────────────────────────

    // Pass raw analogRead() values (0-1023)
    // Scales to signed 16-bit and applies optional deadzone
    void setLeftStick(uint16_t rawX, uint16_t rawY, uint16_t deadzone = 0) {
        input.lx = applyDeadzone(scaleAxis(rawX), deadzone);
        input.ly = applyDeadzone(scaleAxis(rawY), deadzone);
    }

    void setRightStick(uint16_t rawX, uint16_t rawY, uint16_t deadzone = 0) {
        input.rx = applyDeadzone(scaleAxis(rawX), deadzone);
        input.ry = applyDeadzone(scaleAxis(rawY), deadzone);
    }

    // ── Analog triggers ───────────────────────────────────────

    // Pass raw analogRead() values (0-1023)
    // Scales to unsigned 16-bit (0-32736)
    void setTriggers(uint16_t rawL, uint16_t rawR) {
        input.ltrigger = (uint16_t)(rawL * 32UL);
        input.rtrigger = (uint16_t)(rawR * 32UL);
    }

    // ── IMU ───────────────────────────────────────────────────

    // Pass raw int16 values directly from your MPU library
    void setAccel(int16_t x, int16_t y, int16_t z) {
        input.accel[0] = x;
        input.accel[1] = y;
        input.accel[2] = z;
    }

    void setGyro(int16_t x, int16_t y, int16_t z) {
        input.gyro[0] = x;
        input.gyro[1] = y;
        input.gyro[2] = z;
    }

    // ── Trackpads ─────────────────────────────────────────────

    // touched: is a finger on the pad?
    // id:      increment this each time a new touch starts
    // x:       0-1920
    // y:       0-940
    void setTrackpad1(bool touched, uint8_t id, uint16_t x, uint16_t y) {
        input.t1_touch = touched ? 0x01 : 0x00;
        input.t1_id    = id;
        input.t1_x     = x;
        input.t1_y     = y;
    }

    void setTrackpad2(bool touched, uint8_t id, uint16_t x, uint16_t y) {
        input.t2_touch = touched ? 0x01 : 0x00;
        input.t2_id    = id;
        input.t2_x     = x;
        input.t2_y     = y;
    }

    // ── Public report data ────────────────────────────────────

    Gamepad_Input_Report_t  input  = {};  // zero-initialised on startup
    Gamepad_Output_Report_t output = {};

private:

    // Scale analogRead (0-1023) → signed int16 (-32768 to 32767)
    int16_t scaleAxis(uint16_t raw) {
        return (int16_t)((int32_t)raw * 65535L / 1023 - 32768);
    }

    // Zero out values within ±dz of centre (0)
    int16_t applyDeadzone(int16_t val, uint16_t dz) {
        if (dz == 0) return val;
        if (val > -(int16_t)dz && val < (int16_t)dz) return 0;
        return val;
    }

    uint8_t DEBOUNCE(uint8_t pin){
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
};

// Global instance — use  controller.xxx  anywhere in your project
extern SInputHID controller;