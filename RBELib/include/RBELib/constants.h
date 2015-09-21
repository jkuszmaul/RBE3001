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
const float Kp_L, Ki_L, Kd_L;

// Maximum DAC value (for the 0-4096 value).
const int kMaxDAC;

// Arm Information.
const float kArmLen; // Length of each of the arms.
const float kArmHeight; // Height of first arm above the base.
const int t1lowdeg, t1higdeg, t2lodeg, t2higdeg;
const float t1lowrad, t1highrad, t2lowrad, t2highrad;

#endif  //  __CONSTANTS_H__
