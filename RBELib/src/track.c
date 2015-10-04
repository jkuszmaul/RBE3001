#include "RBELib/track.h"
#include "RBELib/constants.h"
#include "RBELib/motors.h"
#include <stdlib.h>

#define QUEUELEN 10

// Circular buffer containing all the weights on the conveyor.
static Weight weightqueue[QUEUELEN];
static char queueend = 0;
static char queuestart = 0; // Which index of the queue we start at.

Weight updateAndGet() {
  // Call monitor functions for near/far IR sensors.
  monitor(0);
  monitor(1);
  cleanQueue();
  return nextWeight();
}

void monitor(char far) {
  // Whether we are currently seeing a block or not.
  static char seeing[2] = {0, 0};
  // The number of consecutive samples which do not conform to the
  // current state.
  static char numchange[2] = {0, 0};
  static char registered[2] = {0, 0};
  // Count for numchange at which we consider the state to have changed.
  static const char kChangeCutoff = 10;
  static unsigned lowest[2] = {-1, -1};
  static unsigned long lowesttime[2] = {0, 0};
  int curdist = IRDist(far ? kFarIR : kNearIR);

  // If relevant, set lowest.
  // When going down (ie, starting to see a weight), increment numchange every
  // consecutive time we are below the cutoff. When going up, every time we
  // don't see a new record low, increment numchange.
  if (seeing[far] && (curdist < lowest[far])) {
    lowest[far] = curdist;
    lowesttime[far] = getHundredths();
    numchange[far] = 0;
  }
  else if (seeing[far]) numchange[far] += 1;
  else if (curdist < kNoWeightCutoff) numchange[far] += 1;
  else numchange[far] = 0;

  // Handle transitions.
  if (numchange[far] > kChangeCutoff) {
    // If we are done seeing a weight, then record it and reset the state.
    if (seeing[far]) {
      if (!registered[far]) {
        if (far)
          registerWeight(lowest[far], lowesttime[far] + kFarIRTime);
        else
          updateWeight(lowest[far], lowesttime[far] + kNearIRTime);
        registered[far] = 1;
      }
      lowest[far] = kNoWeightCutoff;
      if (curdist > kNoWeightCutoff) {
        seeing[far] = 0;
        numchange[far] = 0;
      }
    }
    else {
      registered[far] = 0;
      seeing[far] = 1;
      numchange[far] = 0;
    }
  }

}

void registerWeight(unsigned pos, unsigned long time) {
  // If the queue length is over QUEUELEN, undefined behavior occurs.
  weightqueue[queueend].pos = pos - kConveyorNear;
  if (weightqueue[queueend].pos > 200) weightqueue[queueend].pos = 0;
  weightqueue[queueend].time = time;
  weightqueue[queueend].real = 1;
  queueend = (queueend + 1) % QUEUELEN;
}

void removeWeight() {
  weightqueue[queuestart].real = 0;
  queuestart = (queuestart + 1) % QUEUELEN;
}

void cleanQueue() {
  unsigned long curtime = getHundredths();
  for (int i = queuestart; ; ++i) {
    i %= QUEUELEN;
    if (i == queueend) break;
    // Return if we still have time to go.
    // Includes "grace period".
    if (weightqueue[i].time > curtime + 10) return;
    // Otherwise, remove from queue.
    else removeWeight();
  }
}

Weight nextWeight() {
  return weightqueue[queuestart];
}

void updateWeight(unsigned pos, unsigned long time) {
  // Find closest weight.
  Weight *closest = 0;
  unsigned long lowtimediff = -1;
  for (int i = queuestart; ; ++i) {
    i %= QUEUELEN;
    if (i == queueend) break;
    unsigned long timediff = abs(weightqueue[i].time - time);
    if (timediff < lowtimediff) {
      closest = weightqueue + i;
      lowtimediff = timediff;
    }
  }
  // Do check for null pointer before continuing.
  if (closest) {
    closest->time = time;
    return;
    // For now, ignore the second sensor.
    pos -= kConveyorNear;
    closest->pos = (closest->pos + pos) / 2;
    if (closest->pos > 200) closest->pos = 0;
  }
}
