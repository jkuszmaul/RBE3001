#include "RBELib/forwardkin.h"
#include "RBELib/constants.h"
#include <math.h>

#define PI 3.1415926536

Coord forward(int t1int, int t2int) {
  // Convert to radians.
  float t1 = t1int * PI / 180.0, t2 = t2int * PI / 180.0;
  // Precompute useful sines/cosines.
  float sint1t2 = sin(t1 + t2);
  float cost1t2 = cos(t1 + t2);
  float sint1 = sin(t1);
  float cost1 = cos(t1);
  // Actually calculate position of the end-point.
  float tx = kArmLen * (sint1t2 + sint1);
  float ty = kArmLen * (cost1t2 + cost1) + kArmHeight;
  int x = tx;
  int y = ty;
  Coord out = {.x = x, .y = y};
  return out;
}
