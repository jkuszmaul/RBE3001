#ifndef __FORWARDKIN_H__
#define __FORWARDKIN_H__

/**
 * A struct meant to contain the cartesian coordinates, in mm, of the arm.
 */
typedef struct {
  int x, y; // millimeters
} Coord;

/**
 * Compute the forward kinematics of a pair of joint angles (in degrees).
 * @param t1 The angle of the first joint with respect to the vertical strut.
 * @param t2 The angle of the second joint with respect to the first.
 * @return The (x, y) position with respect to the base of the manipulator (mm).
 */
Coord forward(int t1, int t2);

#endif  //  __FORWARDKIN_H__
