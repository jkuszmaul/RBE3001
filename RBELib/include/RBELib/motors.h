/**
 * @file motors.h
 *
 * @brief Motor driving functions for the arm.
 *
 * @author Eric Willcox
 * @date July 9, 2014
 */

#ifndef MOTORS_H_
#define MOTORS_H_
#include "RBELib/forwardkin.h"
#include "RBELib/ik.h"
#include "RBELib/PID.h"

volatile char isrdone;

/**
 * @brief Initializes PID code for the motors.
 * Note: This makes use of timer 0.
 */
void initMotors();

/**
 * @brief ISR to call to run PID on every iteration.
 */
void motorISR();

/**
 * @brief Write millivolts to both motors.
 * Automatically clips the output to a reasonable range.
 */
void writeMotors(int mlow, int mhigh);

/**
 * @brief Helper function to stop the motors on the arm.
 *
 * @todo Create way to stop the motors using the DAC.
 */
void stopMotors();

/**
 * @brief Drive the arm to a desired angle
 *
 * @param lowerTheta The desired angle for the lower link.
 * @param upperTheta The desired angle for the upper link.
 *
 * @todo Make a way to drive the links to a desired angle.
 */
void gotoAngles(int lowerTheta, int upperTheta);

/**
 * @brief Drive the end effector of the arm to a desired X and Y position in the workspace.
 *
 * @param x The desired x position for the end effector.
 * @param y The desired y position for the end effector.
 *
 * @todo Use kinematic equations to move the end effector to the desired position.
 */
void gotoXY(int x, int y);

/**
 * @brief Drive a link (upper or lower) in a desired direction.
 *
 * @param link Which link to control.
 * @param dir Which way to drive the link.
 *
 * @todo Create a way to drive either link in any direction.
 */
void driveLink(int link, int dir);

/**
 * @brief Drive the arm to a "home" position using the potentiometers.  This
 * should be called before using the encoders and just goes to a default position.
 * Once this has been called once, you can initialize/clear the encoders.
 *
 * @todo Drive the arm to a known position using the potentiometers.
 */
void homePos();

/**
 * @brief Get the current joint positions.
 */
Joint curJoints();

/**
 * @brief Get the current x, y position.
 */
Coord curXY();

/**
 * @brief Get the appropriate joint position.
 */
int getPos(Link link);

#endif /* MOTORS_H_ */
