#include "RBELib/ADC.h"
#include "RBELib/pot.h"
#include "RBELib/constants.h"

int potAngle(int pot) {
  int raw = getADC(pot);
  int angle = (int)((float)(raw + kRawOffset) * kPotRawToAngleScale);
  return angle;
}

int potVolts(int pot) {
  unsigned raw = getADC(pot);
  int millivolts = raw * kPotRawToVolts;
  return millivolts;
}
