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
  const float Kp = (link == kH) ? pidConsts.Kp_H : Kp_L;
  const float Ki = (link == kH) ? pidConsts.Ki_H : Ki_L;
  const float Kd = (link == kH) ? pidConsts.Kd_H : Kd_L;
  const int kMaxSum = kVMotor / Ki;

  const int error = setPoint - actPos;
  const int dError = error - prevError;

  const int out = Kp * error + Ki * sum + Kd * dError;

  // Update kSum and kPrevError.

  // Prevent run-off by not letting kMaxSum get higher than would force the full
  // voltage.
  sum += error;
  if (sum > kMaxSum) sum = kMaxSum;
  if (sum < -kMaxSum) sum = -kMaxSum;

  prevError = error;

  return out;
}

int calcFF(Link link, int setPoint, int actPos) {
}
