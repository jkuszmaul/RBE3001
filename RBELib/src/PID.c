/**
 * @file PID.c
 * @brief The source file for PID functions.
 * @details This contains the implementations for the PID-related functions
 * found in PID.h
 * @author James Kuszmaul
 * @date Septemer 10 2015
 */

#include "RBELib/PID.h"
#include "RBELib/constants.h"
#include "RBELib/util.h"

pidConst pidConsts;

void setConst(Link link, float Kp, float Ki, float Kd) {
  switch (link) {
    case kH:
      pidConsts.Kp_H = Kp;
      pidConsts.Ki_H = Ki;
      pidConsts.Kd_H = Kd;
      break;
    case kL:
      pidConsts.Kp_L = Kp;
      pidConsts.Ki_L = Ki;
      pidConsts.Kd_L = Kd;
      break;
  }
}

int calcPID(Link link, int setPoint, int actPos) {
  static int sum = 0, prevError = 0;
  const float Kp = (link == kH) ? pidConsts.Kp_H : pidConsts.Kp_L;
  const float Ki = (link == kH) ? pidConsts.Ki_H : pidConsts.Ki_L;
  const float Kd = (link == kH) ? pidConsts.Kd_H : pidConsts.Kd_L;
  const int kMaxSum = kVMotor / Ki;

  const int error = setPoint - actPos;
  const int dError = (float)(error - prevError) / kdt;

  float out = Kp * error + Ki * sum + Kd * dError;
  out = clipf(out, -kVMotor, kVMotor);

  // Update kSum and kPrevError.

  // Prevent run-off by not letting kMaxSum get higher than would force the full
  // voltage.
  const int newsum = sum + error * kdt;
  sum = clipi(newsum, -kMaxSum, kMaxSum);

  prevError = error;

  return out;
}

int calcFF(Link link, int setPoint, int actPos) {
  // Rudimentary linear approximation.
  const int kHoldZero = 700;
  if (link == kH) {
    // Get absolute value.
    actPos = (actPos > 0) ? actPos : -actPos;
    const int FF = kHoldZero * (90.0 - actPos) / 90.0; // Linear approx.
    return FF;
  }
  return 0;
}
