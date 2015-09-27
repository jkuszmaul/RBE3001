/**
 * @file pot.h
 *
 * @brief The header file and function prototypes for the potentiometers.
 * @details  Use these functions to read the values from the pots.
 * @author Eric Willcox
 * @date August 17 2013
 */

#ifndef POT_H_
#define POT_H_

/**
 * @brief Find the angle of the given potentiometer.
 * @param  pot The pot port to check.
 * @return angle Angle (in hundredths of a degree) of the potentiometer.
 * @todo Allow custom configurations for different pots.
 */
int potAngle(int pot);

/**
 * @brief Find the voltage value of the given potentiometer.
 * @param  pot The pot port to get the value of.
 * @return volts Voltage (in millivolts) of  potentiometer.
 */
int potVolts(int pot);

#endif /* PERIPH_H_ */
