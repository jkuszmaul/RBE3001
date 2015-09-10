#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#ifndef PI
#define PI 3.14159265358979
#endif  // PI

// Raw pot to hundredths of radians.
const float kPotRawToAngleScale = 180.0 / (1010 - 219);
// Amount that needs to be added to raw before multiplying by
// kPotRawToAngleScale in order to get the real angle.
const int kRawOffset = -620;
const int kPotRawToVolts = 5000 / 1023;

// Motor voltage (millivolts).
const int kVMotor = 8000;
#endif  //  __CONSTANTS_H__
