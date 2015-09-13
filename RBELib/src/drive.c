#include "RBELib/drive.h"

#include "RBELib/constants.h"
#include "RBELib/DAC.h"
#include "RBELib/ADC.h"

void setMotorVolts(unsigned char motor, long mvolts) {
  // Scale such that we output +mvolts/2 and -mvolts/2.
  int DACdiff = (mvolts * 4095) / (kVMotor);
  int positive = DACdiff > 0;
  char dac0 = motor * 2;
  char dac1 = dac0 + 1;
  setDAC(dac0, positive ? DACdiff : 0);
  setDAC(dac1, positive ? 0 : -DACdiff);
}

int getCurrent(char cursen) {
  long raw = getADC(cursen);
  long ma = raw * 5000 / 1023 - 2700;
  return ma;
}
