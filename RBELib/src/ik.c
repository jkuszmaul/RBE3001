#include "RBELib/ik.h"
#include "RBELib/constants.h"
#include "RBELib/motors.h"
#include <math.h>

int inBounds(float t1, float t2) {
  if (isnan(t1) || isnan(t2)) return 0;
  if (t1 < t1lowrad || t1 > t1highrad) return 0;
  if (t2 < t2lowrad || t2 > t2highrad) return 0;
  return 1;
}

Joint getJoint(int x, int y) {
  x *= -1; // Make consistent with FK.
  y -= kArmHeight; // Make y=0 be at pivot of first joint.
  float thetadesired = atan2(y, x);
  float arm2 = kArmLen * kArmLen;
  float r2 = (long)x * x + (long)y * y;
  float r = sqrt(r2);
  // tr=angle between links in triangle.
  float trplus = acos((arm2 + arm2 - r2) / (2 * arm2));
  float trneg = -trplus;
  // tb=angle between first link and position vector.
  float tbplus = acos(r / (2.0 * kArmLen));
  float tbneg = -tbplus;
  // The pair of possible angles for the second joint.
  float t2plus = PI - trplus;
  float t2neg = -PI - trneg;
  // t1plus and t1neg are not necessarily positive and negative; they just
  // correspond to positive and negative tb's.
  // The pair of possible angles for the first joint.
  float t1plus = thetadesired + tbplus - PI / 2;
  float t1neg = thetadesired + tbneg - PI / 2;
  if (t1plus > PI) t1plus -= 2 * PI;
  else if (t1plus < -PI) t1plus += 2 * PI;
  if (t1neg > PI) t1neg -= 2 * PI;
  if (t1neg < -PI) t1neg += 2 * PI;
  // When tb is positive, t2 should be negative (>PI) and vice-versa.
  // So, the two feasible pairs are (t1plus, t2neg) and (t1neg, t2plus).
  if (inBounds(t1plus, t2neg)) {
    int t1 = t1plus * 180.0 / PI * 100.0;
    int t2 = t2neg * 180.0 / PI * 100.0;
    Joint retval = {.t1=t1, .t2=t2, .feasible=1};
    return retval;
  }
  if (inBounds(t1neg, t2plus)) {
    int t1 = t1neg * 180.0 / PI * 100.0;
    int t2 = t2plus * 180.0 / PI * 100.0;
    Joint retval = {.t1=t1, .t2=t2, .feasible=1};
    return retval;
  }

  Joint retval = {0, 0, 0};
  return retval;
}

void conveyorPos(int x) {
  gotoXY(-(x + kConveyorOffset), kWeightHeight + x / 4);
}

void liftArm(int x) {
  gotoXY(-(kConveyorOffset + x), kWeightHeight + 40);
}
