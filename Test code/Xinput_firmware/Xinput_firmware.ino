#include <Wire.h>
#include <XInput.h>

class Xcont {

public:

  Xcont(int deadZone, int joyMidPoint) {
    dZ = deadZone;
    mP = joyMidPoint;
  }

  Xcont(const int fBut[4], const int dP[4], const int lS[2], const int rS[2], int deadZone, int joyMidPoint) {
    int faceBut[4] = {buts[1],buts[2],buts[3],buts[4]};
    int dPad[4] = {buts[11],buts[12],buts[13],buts[14]};
    for (int i = 0; i < 4; i++) {
      faceBut[i] = fBut[i];
      dPad[i] = dP[i];
    }
    for (int i = 0; i < 2; i++) {
      lStick[i] = lS[i];
      rStick[i] = rS[i];
    }
    dZ = deadZone;
    mP = joyMidPoint;
  }

  void setFaceButtons(int a, int b, int x, int y) {
    buts[1] = a;
    buts[2] = b;
    buts[3] = x;
    buts[4] = y;
  }

  int getFaceButttons() {
    int faceBut[4] = {buts[1],buts[2],buts[3],buts[4]};
    return faceBut;
  }

  void setDPad(int up, int down, int left, int right) {
    buts[11] = up;
    buts[12] = down;
    buts[13] = left;
    buts[14] = right;
  }

  int getDPad() {
    int dPad[4] = {buts[11],buts[12],buts[13],buts[14]};
    return dPad;
  }

  void setStick(XInputControl id, int x, int y, int dz, int mp) {
    if (id == JOY_LEFT) {
      lStick[0] = x;
      lStick[1] = y;
      dZ = dz;
      mP = mp;
    } else if (id == JOY_RIGHT) {
      rStick[0] = x;
      rStick[1] = y;
      dZ = dz;
      mP = mp;
    }
  }

  void setStick(XInputControl id, int x, int y) {
    setStick(id, x, y, dZ, mP);
  }

  int getStick(XInputControl id) {

    return;
  }

  void setDeadZone(int dz) {
    dZ = dz;
  }

  int getDeadZone() {
    return dZ;
  }

  void setJoyMidPoint(int mp) {
    mP = mp;
  }

  int getJoyMidPoint() {
    return mP;
  }

  void bind(XInputControl id, int* var, int size) {
    if (id < 17) {
      XInput.setButton(id, var);
    } else if (size  < 3 ) {
    }
    else if (size >=3 && size  <5){
      
    }
    else if (size >=5 && size  <6){}
  }

private:
  bool state;

  int dZ, mP, inti = 0;
  int buts[20];              // [MENU, A, B, X, Y, L1, R1, SELECT, START, L3, R3, UP, DOWN, LEFT, RIGHT, L2, R2, SP1, SP2, SP3, SP4}
  int dPad[4];               // [Up, Down, Left, Right]
  int lStick[6], rStick[6];  // [X, Y, deadzone, mid point]
  int LRBut[6];              // [L1, L2, L3, R1, R2, R3]
};

void setup() {
  // put your setup code here, to run once:
  XInput.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}
