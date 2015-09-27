/**
 * @file Periph.h
 *
 * @brief The header file and function prototypes for the peripherals (IR, encoder and accelerometer).
 * @details Each of these functions is for controlling the peripheral devices of the arm.
 * @author Eric Willcox
 * @date August 21, 2013
 * @date July 18, 2014
 */

#ifndef PERIPH_H_
#define PERIPH_H_
#include "RBELib/PID.h"

#ifndef NULL
#define NULL 0
#endif  // NULL

/**
 * @brief Find the acceleration in the given axis (X, Y, Z).
 * @param  axis The axis that you want to get the measurement of.
 * @return gVal Value of  acceleration.
 *
 * @todo Create a function that is able to find the acceleration of a given axis.
 */
signed int getAccel(int axis);

void resetAccelRef();
signed int getAccelRaw(int axis);

/**
 * @brief Read an IR sensor and calculate the distance of the block.
 * @param  chan The port that the IR sensor is on.
 * @return value The distance the block is from the sensor.
 *
 * @todo Make a function that is able to get the ADC value of the IR sensor.
 */
int IRDist(int chan);

/**
 * @brief Initialize the encoders with the desired settings.
 * @param chan Channel to initialize.
 *
 * @todo Make a function that can setup both encoder chips on the board.
 */
void encInit(int chan);

/**
 * @brief Reset the current count of the encoder ticks.
 * @param chan The channel to clear.
 *
 * @todo Clear the encoder count (set to 0).
 */
void resetEncCount(int chan);

/**
 * @brief Finds the current count of one of the encoders.
 * @param  chan Channel that the encoder is on that you would like to read.
 * @return count The current count of the encoder.
 *
 * @todo Find the current encoder ticks on a given channel.
 */
signed long encCount(int chan);

/**
 * @brief convert encoder counts to angle.
 */
int encAngle(Link link);

typedef enum {
  kClr=0,
  kRd=1,
  kWr=2,
  kLd=3,
} EncCmd;

typedef enum {
  kNone=0,
  kMDR0=1,
  kMDR1=2,
  kDTR=3,
  kCNTR=4,
  kOTR=5,
  kSTR=6,
} EncReg;

void encCmd(unsigned char cs, unsigned char cmd, unsigned char reg,
            unsigned char *bytes, unsigned char numBytes);

#endif /* PERIPH_H_ */
