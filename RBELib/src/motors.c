/**
 * @file motors.c Contains functions to more easily use PID and IK.
 * @author James Kuszmaul
 * @date 25 September 2015
 */
#include "RBELib/motors.h"
#include "RBELib/timer.h"
#include "RBELib/drive.h"
#include "RBELib/constants.h"
#include "RBELib/util.h"
#include "RBELib/pot.h"

static Joint goaljoint = {.t1=0, .t2=0};
static Coord curxy = {.x=0, .y=300};
static Joint curjoint = {.t1=0, .t2=0};
static char enable = 0; // Whether or not to drive the motors.
static char reset = 1; // Whether or not to reset PID on next iteration.

void initMotors() {
  setConst(kL, Kp_H, Ki_H, Kd_H);
  setConst(kH, Kp_L, Ki_L, Kd_L);
  initTimer(0, CTC, 10000 /*10000us = 100Hz*/);
  setTimerInterrupt(0, motorISR);
  writeMotors(0, 0);
}

volatile char isrdone = 0;
void motorISR() {
  isrdone = 1;
  if (!enable) {
    writeMotors(0, 0);
    return;
  }

  curjoint.t1=getPos(kL);
  curjoint.t2=getPos(kH);

  int mlow =
      calcPID(kL, goaljoint.t1, curjoint.t1, reset) + calcFF(kL, curjoint);
  int mhigh =
      calcPID(kH, goaljoint.t2, curjoint.t2, reset) + calcFF(kH, curjoint);

  writeMotors(mlow, mhigh);

  curxy = forward(curjoint.t1, curjoint.t2);

  reset = 0;
}

void writeMotors(int mlow, int mhigh) {
  // Clip outputs to a reasonable range.
  mlow = clipi(mlow, -kVMotor, kVMotor);
  mhigh = clipi(mhigh, -kVMotor, kVMotor);
  // The top motor is inverted.
  setMotorVolts(kH, -mhigh);
  setMotorVolts(kL, mlow);
}

inline int valInBounds(int val, int low, int high) {
 return (val > low) && (val < high);
}

int getPos(Link link) {
  static Joint useEnc = {0, 0}; // Whether to use the encoder for each link.
  int pot, enc;
  const int homeCutoff = -1;
  switch (link) {
    case kH:
      if (useEnc.t2 == 0) {
        pot = potAngle(kPotHigh);
        if (valInBounds(pot, -homeCutoff, homeCutoff)) {
          useEnc.t2 = 1;
          resetEncCount(kH);
        }
        return pot;
      }
      break;
    case kL:
      if (useEnc.t2 == 0) {
        pot = potAngle(kPotLow);
        if (valInBounds(pot, -homeCutoff, homeCutoff)) {
          useEnc.t1 = 1;
          resetEncCount(kL);
        }
        return pot;
      }
      break;
  }
  return encAngle(link);
}

void gotoXY(int x, int y) {
  Joint joints = getJoint(x, y);
  reset = 1;
  enable = 1;
  // Fail silently if not a feasible position.
  if (joints.feasible) {
    goaljoint = joints;
  }
}

void gotoAngles(int lowerTheta, int upperTheta) {
  goaljoint.t1 = lowerTheta;
  goaljoint.t2 = upperTheta;
  reset = 1;
  enable = 1;
}
