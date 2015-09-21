/*
 * main.c
 *
 * Created on: Aug 6, 2014
 * Author: ewillcox
 */
#include "RBELib/RBELib.h" //RBELib

static const int ain = 2;


/**
 * Runs two DACs up and down over its full range.
 */
void triangleDAC() {
  for (int i = 0; i < 4096; i+=16) {
    setDAC(0, i);
    setDAC(1, 4095 - i);
  }
  for (int i = 4095; i >= 0; i-=16) {
    setDAC(0, i);
    setDAC(1, 4096 - i);
  }
}

// Functions for tuning PID loops; these read ADC values and scale them into a
// useful range for actual constants.
float getP() {
  int raw = getADC(4);
  float kP = (float)raw * 8000.0 / 2.0 / 1023.0;
  return kP;
}

float getI() {
  int raw = getADC(5);
  float kI = (float)raw * 0.1;
  return kI;
}

float getD() {
  int raw = getADC(6);
  float kD = (float)raw * 0.0001;
  return kD;
}

// Various useful state variables.
static volatile int x, y; // Goal position, in mm (passed to getJoint()).
static volatile long totall, totalh; // The motor output values, in mV.
// Used for tracking the number of pid iteratiosn that have passed.
static volatile int count = 0;

// The points to use to draw the triangle.
static Coord p1 = {220, 0}, p2 = {260, 0}, p3={240, 40};

// The different modes, for different parts of the lab.
// These only affect how the X-Y position is chosen for doing IK.
enum {
  kTrace, // Trace a triangle using points along each edge.
  kTriangle, // Go between three setpoints without tracing.
  kPoints, // Use switches to go between different positions.
  kPots // Use pots to control x-y position.
} mode;

void runPID() {
  ++count;
  // Change mode dependong on part of the lab.
  mode = kPoints;
  // Consider converting in some way to using interrupts or speeding up the ADC.
  int posl = potAngle(kPotLow);
  int posh = potAngle(kPotHigh);

  // Whether or not to restart the PID calculation (ie, clear the error sum).
  int reset = 0;

  switch (mode) {
    case kTrace:
      // Every 100 counts, change which point we are aiming for.
      // Every 10 counts, update the PID setpoint.
      if (count > 300 || count < 100) {
        if (count > 300) count = 0;
        if ((count % 10) == 0) {
          x = p3.x + (p1.x - p3.x) * count / 100.0;
          y = p3.y + (p1.y - p3.y) * count / 100.0;
          reset = 1;
        }
      }
      else if (count >= 200) {
          if ((count % 10) == 0) {
            x = p2.x + (p3.x - p2.x) * (count - 200) / 100.0;
            y = p2.y + (p3.y - p2.y) * (count - 200) / 100.0;
            reset = 1;
          }
      }
      else if (count >= 100) {
        if ((count % 10) == 0) {
          x = p1.x + (p2.x - p1.x) * (count - 100) / 100.0;
          y = p1.y + (p2.y - p1.y) * (count - 100) / 100.0;
          reset = 1;
        }
      }
      break;

    case kTriangle:

      // Every 100 counts, change which point we are aiming for.

      if (count > 300 || count < 100) {
        if (count > 300) count = 0;
        x = p1.x;
        y = p1.y;
        reset = 1;
      }
      else if (count >= 200) {
        x = p3.x;
        y = p3.y;
        if (count == 200) reset = 1;
      }
      else if (count >= 100) {
        x = p2.x;
        y = p2.y;
        if (count == 100) reset = 1;
      }
      break;

    case kPoints:
      // Use one switch for switching between two x-coordinates.
      x = 220 + 40 * ((PINB & 2) >> 1);
      // Use the other for y-coordinates.
      y = 50 * ((PINB & 4) >> 2);
      break;

    case kPots:
      // Scale the raw pot values into a reasonable range.
      x = getADC(4) * .6 - 300;
      y = getADC(5) / 2 - 200;
      break;
  }

  // Perform the actual IK.
  // Offest y so that y=0 is at pivot of low joint.
  Joint joints = getJoint(x, y + kArmHeight);

  // PID angle setpoints for low and high joints.
  static int setpointl=0, setpointh=0;

  // For debugging.
  //printf("x %d y %d t1 %d t2 %d f %d\n", x, y, joints.t1, joints.t2, joints.feasible);

  // Only change the setpoints if the IK worked.
  if (joints.feasible) {
    setpointl = joints.t1;
    setpointh = joints.t2;
  }

  // Perform PID and gravity compensation calculations.
  totall = calcPID(kL, setpointl, posl, reset) + calcFF(kH, setpoint, pos);
  totalh = -(calcPID(kH, setpointh, posh, reset) + calcFF(kL, setpoint, pos));
  // Clip outputs to a reasonable range.
  totall = clipi(totall, -kVMotor, kVMotor);
  totalh = clipi(totalh, -kVMotor, kVMotor);
   //printf("%d\n", getCurrent(0)); // More debugging.

  // Actually set the motor output.
  setMotorVolts(kH, totalh);
  setMotorVolts(kL, totall); // TODO: Document invertedness of motors.

  // Used by lab; output arm position information.
  Coord pos = forward(posl, posh);
  printf("%d %d %d %d %d %d\n", posl, posh, pos.x, pos.y, x, y);
}

// Set up PID constants and timer.
// TODO: break into library function.
void initPID(int pos) {
  setConst(kL, Kp_H, Ki_H, Kd_H);
  setConst(kH, Kp_L, Ki_L, Kd_L);
  initTimer(0, CTC, 10000 /*10000us = 100Hz*/);
  DDRB &= ~(0x06); // Connected to switches on board.
  x = 0;
  y = 300;
  setTimerInterrupt(0, runPID);
}

// Print current arm position.
void printADC() {
  int potangle1 = potAngle(kPotLow);
  int potangle2 = potAngle(kPotHigh);
  Coord pos = forward(potangle1, potangle2);
  printf("%d %d %2.2f %2.2f\n", potangle1, potangle2, pos.x, pos.y);
}

// Print current usage by a motor.
void printCur() {
  // Formerly, ramped motor down->up to vary current.
  for (int i = -kVMotor; i < kVMotor; i+=10) {
    _delay_us(100);
    printf("%d, %d\n", i, getCurrent(0));
  }
}

// Call various init functions.
void initAll() {
  initRBELib();
  debugUSARTInit(115200);
  initSPI();
  initADC(ain, 1, 0);
}

// Used for debugging IK/FK equations.
void printJoints(int x, int y) {
  Joint j = getJoint(x, y);
  printf("x %d y %d t1 %d t2 %d f %d\n", x, y, j.t1, j.t2, j.feasible);
  Coord fk = forward(j.t1, j.t2);
  printf("Forward: x %d y %d\n", fk.x, (int)(fk.y - 5.62 * 25.4));
}

int main() {
  initAll();
  printf("HELLO.\n");
  // Initialize PID loops and start timers.
  initPID(0);
  // Test the IK functions.
  printJoints(0, 60);
  printJoints(0, 150);
  printJoints(0, 300);
  printJoints(100, 200);

  // Uncomment various things to do different parts of the lab.
  while (1) {
    //printf("\n");
    //printCur();
    //triangleDAC();
    //printADC();
    //_delay_ms(10);
  }
  return 0;
}

