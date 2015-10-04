/**
 * @file drive.h
 * @brief This file contains code to set the voltages directly to the motors, as
 * well as related utility functions (namely, reading the current sensors).
 * @author James Kuszmaul
 * @date 10 September 2015
 */
#ifndef __DRIVE_H__
#define __DRIVE_H__

/**
 * @brief Sets the specified motor to a certain voltage, in millivolts.
 * @param motor motor to set (0 or 1).
 * @param mvolts the voltage to set, in millivolts.
 */
void setMotorVolts(unsigned char motor, long mvolts);

/**
 * @brief Gets the current, in milli-Amperes, detected by the current sensors.
 * @param cursen The number of the current sensor to read.
 * @return The current current measured by the sensor, in mA.
 */
int getCurrent(char cursen);

/**
 * Integrates and averages the current for a given sensor and resets the
 * integration to zero of asked.
 * This must be called at some regular interval in order to work.
 * Returns the most recent average for the sensor in question.
 */
int getAvgCurrent(char cursen, char reset);

#endif  // __DRIVE_H__
