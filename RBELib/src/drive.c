#include "RBELib/drive.h"

#include "RBELib/constants.h"
#include "RBELib/DAC.h"
#include "RBELib/ADC.h"

void setMotorVolts(unsigned char motor, long mvolts) {
  // Scale such that we output +mvolts/2 and -mvolts/2.
  int DACdiff = (mvolts * kMaxDAC) / (kVMotor);
  int positive = DACdiff > 0;
  printf("In set motor volts %d\n", __LINE__);
  char dac0 = motor * 2;
  char dac1 = dac0 + 1;
  printf("In set motor volts %d\n", __LINE__);
  setDAC(dac0, positive ? DACdiff : 0);
  printf("In set motor volts %d\n", __LINE__);
  setDAC(dac1, positive ? 0 : -DACdiff);
  printf("In set motor volts %d\n", __LINE__);
}

int getCurrent(char cursen) {
  long raw = getADC(cursen);
  long ma = raw * 5000 / 1023 - 2700;
  return ma;
}
