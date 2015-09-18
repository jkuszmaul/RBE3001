#include "RBELib/constants.h"

// Raw pot to hundredths of radians.
const float kPotRawToAngleScale = 180.0 / (1010 - 219);
// Amount that needs to be added to raw before multiplying by
// kPotRawToAngleScale in order to get the real angle.
const int kRawOffset = -620;
const int kPotRawToVolts = 5000 / 1023;

// Motor voltage (millivolts).
const int kVMotor = 8000;

// Maximum DAC value (for the 0-4096 value).
const int kMaxDAC = (int)((4095UL * 8000UL) / (2500UL * 6));

// PID Constants
const float kdt = 0.01;
const float Kp_H = 742, Ki_H = 5.2, Kd_H = 0.06;
