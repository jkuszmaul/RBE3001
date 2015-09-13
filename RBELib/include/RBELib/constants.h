#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#ifndef PI
#define PI 3.14159265358979
#endif  // PI

// Raw pot to hundredths of radians.
const float kPotRawToAngleScale;
// Amount that needs to be added to raw before multiplying by
// kPotRawToAngleScale in order to get the real angle.
const int kRawOffset;
const int kPotRawToVolts;

// Motor voltage (millivolts).
const int kVMotor;

// PID Constants
const float kdt;
const float Kp_H, Ki_H, Kd_H;
#endif  //  __CONSTANTS_H__
