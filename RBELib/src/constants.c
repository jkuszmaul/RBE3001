#include "RBELib/constants.h"

// Raw pot to hundredths of radians.
const float kPotRawToAngleScale = 180.0 / (1010 - 219);
// Amount that needs to be added to raw before multiplying by
// kPotRawToAngleScale in order to get the real angle.
const int kRawOffset = -620;
const int kPotRawToVolts = 5000 / 1023;

// Ports corresponding to various analog inputs.
const char kPotLow = 3, kPotHigh = 2;

// Motor voltage (millivolts).
const int kVMotor = 8000;

// Maximum DAC value (for the 0-4096 value).
const int kMaxDAC = (int)((4095UL * 8000UL) / (2500UL * 6.1));

// PID Constants
const float kdt = 0.01;
const float Kp_H = 780, Ki_H = 5.1, Kd_H = 0.06;
const float Kp_L = 780, Ki_L = 5.1, Kd_L = 0.06;

// Arm Constants.
const float kArmLen = 6.0 * 25.4;
const float kArmHeight = 5.62 * 25.4;
const int t1lodeg = -106, t1higdeg = 90, t2lodeg = -114, t2higdeg = 90;

const float t1lowrad = -106 * PI / 180, t1highrad = PI / 2,
            t2lowrad = -114 * PI / 180.0, t2highrad = PI / 2;
