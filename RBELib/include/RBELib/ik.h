#ifndef __IK_H__
#define __IK_H__

typedef struct {
  int t1;
  int t2;
  char feasible;
} Joint;

/**
 * Do the inverse kinematics for a given manipulator (x, y).
 * If the IK isn't possible, then the return value's feasible field will be 0.
 * The y axis points up along the fixed vertical base of the arm.
 * The x axis points right along the base of the robot (ie, the table).
 * @param x The x position, in mm.
 * @param y The y position, in mm.
 * @return The joint positions, in degrees.
 */
// TODO: Return the closest reasonable solution when infeasible, so that
// we don't completely ruin everything when we don't care about it.
Joint getJoint(int x, int y);

/**
 * Check to see if a joint position is feasible (namely, that t1 and t2 are not
 * outside of the range -PI/2 to +PI/2). Uses the t[12](low|high)rad constants.
 * @param t1,t2 Joint positions in radians.
 * @return 1 if inbounds, 0 if not.
 */
int inBounds(float t1, float t2);

/**
 * Put the arm at a given position over the conveyor to grab a weight.
 * @param x Position, in mm, side-to-side on conveyor.
 */
void conveyorPos(int x);

/**
 * Lifts the arm a bit above the conveyor.
 */
void liftArm(int x);

#endif  // __IK_H__
