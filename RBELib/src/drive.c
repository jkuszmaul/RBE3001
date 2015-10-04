#include "RBELib/drive.h"

#include "RBELib/constants.h"
#include "RBELib/DAC.h"
#include <avr/io.h>
#include <stdlib.h>
#include "RBELib/ADC.h"

void setMotorVolts(unsigned char motor, long mvolts) {
  // Scale such that we output +mvolts/2 and -mvolts/2.
  int DACdiff = (mvolts * kMaxDAC) / (kVMotor);
  int positive = DACdiff > 0;
  char dac0 = motor * 2;
  char dac1 = dac0 + 1;
  // Always keep one DAC at GND and the other at a non-zero voltage.
  setDAC(dac0, positive ? DACdiff : 0);
  setDAC(dac1, positive ? 0 : -DACdiff);
}

int getCurrent(char cursen) {
  long raw = getADC(cursen);
  long ma = raw * 5000 / 1023 - 2700;
  return ma;
}

int getAvgCurrent(char cursen, char reset) {
  static long total[2] = {0, 0};
  static int cnt[2] = {0, 0};
  if (reset) {
    total[cursen] = 0;
    cnt[cursen] = 0;
  }
  int cur = getCurrent(cursen);
  total[cursen] += -cur;
  ++cnt[cursen];
  return total[cursen] / cnt[cursen];
}
