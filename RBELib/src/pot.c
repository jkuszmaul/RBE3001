#include "RBELib/ADC.h"
#include "RBELib/pot.h"
#include "RBELib/constants.h"

int potAngle(int pot) {
  int raw = getADC(pot);
  int angle;
  if (pot == kPotHigh)
    angle = (int)((float)(raw + kRawOffsetH) * kPotRawToAngleScaleH * 100);
  else
    angle = (int)((float)(raw + kRawOffsetL) * kPotRawToAngleScaleL * 100);
  return angle;
}

int potVolts(int pot) {
  unsigned raw = getADC(pot);
  int millivolts = raw * kPotRawToVolts;
  return millivolts;
}
