#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#ifndef PI
#define PI 3.14159265358979
#endif  // PI

// Raw pot to hundredths of radians.
const float kPotRawToAngleScaleL;
const float kPotRawToAngleScaleH;
// Amount that needs to be added to raw before multiplying by
// kPotRawToAngleScale in order to get the real angle.
const int kRawOffsetL, kRawOffsetH;
const int kPotRawToVolts;
// Scalar to convert encoder counts to hundredths of degrees.
const float kEncToDegL, kEncToDegH;

// Ports corresponding to various analog inputs.
const char kPotLow, kPotHigh;

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

// Constants for where to position the arm relative to the conveyor.
const int kWeightHeight, kConveyorOffset;

// Ports for the servos.
const char kServoGrab, kServoConveyor;

// Constants for servo open/close.
const char kGrabOpen, kGrabClose;

// Ports for IR sensors;
const char kFarIR, kNearIR;

// IR distance constants.
const int kNoWeightCutoff;
const int kConveyorNear;
const float kConveyorScale;

// Time between passing the IR sensors and reaching the arm.
const int kFarIRTime, kNearIRTime; // hundredths of a second.

// time before arrival of a weight to start actuating.
const unsigned long kArmMoveTime, kGripMoveTime;

// Cutoff for weights.
const unsigned int kWeightCutoff;

#endif  //  __CONSTANTS_H__
