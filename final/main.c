/*
 * main.c
 *
 * Created on: Aug 6, 2014
 * Author: ewillcox
 */
#include "RBELib/RBELib.h" //RBELib

// Call various init functions.
void initAll() {
  initRBELib();
  debugUSARTInit(115200);
  initSPI();
  initADC(kPotHigh, 0, 0);
  setSPIClkDivide(kSPI4);
  DDRB &= ~0x6;
  setServo(kServoConveyor, 0);
  setSPIClkDivide(kSPI4);
 // initTimer(2, CTC, 10000);
 // setTimerInterrupt(2, isr);
 // _delay_ms(5); // Don't start timer ISRs too close to one another.
  initMotors();
}

typedef enum {
  kWaiting, // Wait until a weight gets close enough to start moving.
  kGripping,
  kLifting,
  kMeasuring,
  kDumping,
} State;

int main() {
  // Initialize PID loops and start timers.
  initAll();
  printf("HELLO.\n");
  conveyorPos(0);
  State state = kWaiting;
  unsigned long nextEvent = getHundredths();
  while (1) {
    // Run at ~100Hz.
    while (!isrdone) continue;
    isrdone = 0;
   //if (PINB & 0x02) servoGrab(1); else servoGrab(0);
  //  if (!(PINB & 0x04)) {
    	//int x = (int)getADC(6) / 5 - 300;
    	//int y = getADC(7) / 5;
    	//int pos = getADC(6) / 10;
    	//printf("%d, %d\n", x, y);
//    }
    // Tasks which must be done every iteration.
    // Run the IR sensors.
    Weight nextWeight = updateAndGet();
    //printf("%d %d\n", state, nextWeight.pos);
    unsigned long time = getHundredths();
    int cur1 = getAvgCurrent(1, 0);
    int cur0 = getAvgCurrent(0, 0);
    switch (state) {
      case kWaiting:
    	if (nextWeight.real == 0) break;
        liftArm(nextWeight.pos* kConveyorScale);
        servoGrab(0);
        if (nextWeight.time < (time + kArmMoveTime)) state = kGripping;
        break;
      case kGripping:
        conveyorPos(nextWeight.pos * kConveyorScale);
        if (nextWeight.time < (time - 150)) {
          state = kLifting;
          // Time at which we will shift from lifting to actually dumping it.
          nextEvent = time + 300;
         // servoGrab(0);
        }
        else if (nextWeight.time < (time + kGripMoveTime)) servoGrab(1);
        break;
      case kLifting:
        //liftArm(30);
    	gotoAngles(0, -9000);
        if (time > nextEvent) {
          nextEvent = time + 300;
          // Reset current averaging.
          cur1 = getAvgCurrent(1, 1);
          cur0 = getAvgCurrent(0, 1);
          state = kMeasuring;

        }
        break;
      case kMeasuring:
      //	gotoAngles(0, -9000);
    	  writeManual(0, 3000);

          if (time > nextEvent) {
            nextEvent = time + 100;
            state = kDumping;
        //    printf("cur: %d %d\n", cur1, cur0);
            if (cur0 < kWeightCutoff) {
              gotoAngles(-9000, 0);
            }
            else {
              liftArm(30);
              //nextEvent
            }
          }
    	  break;
      case kDumping:
        if (time > nextEvent) {
        	state = kWaiting;
        	servoGrab(0);
        }
        break;
    }

    printf("%d %d\n", potAngle(3), potAngle(2));
    /*
    if (PINB & 0x02) {
      conveyorPos(0);
    }
    else {
      liftArm(0);
    }
    servoGrab(PINB & 0x4);
    printf("%d, %d\n", IRDist(4), IRDist(5));
    */
  }
  return 0;
}

