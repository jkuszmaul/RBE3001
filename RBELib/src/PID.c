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

int calcPID(Link link, int setPoint, int actPos, int reset) {
  static int suml = 0, sumh = 0;
  static float prevError = 0;
  if (reset) {
    if (link == kH) sumh = 0;
    else suml = 0;
  }
  const float Kp = (link == kH) ? pidConsts.Kp_H : pidConsts.Kp_L;
  const float Ki = (link == kH) ? pidConsts.Ki_H : pidConsts.Ki_L;
  const float Kd = (link == kH) ? pidConsts.Kd_H : pidConsts.Kd_L;
  int * sum = (link == kH) ? &sumh : &suml;
  const int kMaxSum = kVMotor / Ki;

  const float error = (setPoint - actPos) / 100;
  const int dError = (float)(error - prevError) / kdt;

  float out = Kp * error + Ki * *sum + Kd * dError;
  out = clipf(out, -kVMotor, kVMotor);

  // Update kSum and kPrevError.

  // Prevent run-off by not letting kMaxSum get higher than would force the full
  // voltage.
  const int newsum = *sum + error * kdt;
  *sum = clipi(newsum, -kMaxSum, kMaxSum);

  prevError = error;

  return out;
}

int calcFF(Link link, Joint pos) {
  // TODO: Tune Gravity Compensation.
  const int kHoldZeroH = 700, kHoldZeroL = 700;
  int hold = (link == kH) ? kHoldZeroH : kHoldZeroL;
  int anglesum = pos.t1 + pos.t2;
  if (link == kH) {
    // Gravity comp is just negative sine of angle.
    float anglerad = anglesum / 100.0 * PI / 180.0;
    const int FF = -kHoldZeroH * sin(anglerad);
    return FF;
  }

  // We will assume that the bottom's feed-forward is independent of
  // the top's position.
  if (link == kL) {
    float anglerad = pos.t1 / 100.0 * PI / 180.0;
    const int FF = -kHoldZeroL * sin(anglerad);
    return FF;
  }

  return 0;
}
