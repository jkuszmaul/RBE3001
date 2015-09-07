/**
 * @file timer.h
 *
 * @brief The header file and function prototypes for Timers 0-2.
 *
 * @author Justin Barrett
 * @date August 25, 2011
 *
 * @author Eric Willcox
 * @date August 20, 2013
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "RBELib/util.h"

/**
 * @brief Timer normal mode.
 */
#define NORMAL 0
/**
 * @brief Timer Clear Timer on Compare (CTC) mode.
 */
#define CTC 2

/**
 * The bitmasks corresponding to different prescalings. See p100.
 */
enum PreScale {
  k1=1,
  k8=2,
  k64=3,
  k256=4,
  k1024=5,
};

/**
 * @brief Initializes the specified timer in the specified mode. This
 * header file provides constants for NORMAL operation mode and
 * CTC operation mode, however there are many more modes of operation
 * for the Timers that can be experimented with.
 *
 * @param timer The number of the timer to be initialized (0-2).
 * @param mode 0 if normal, 2 if CTC mode. No other modes currently supported.
 * @param comp Only used in CTC mode. The period, in microseconds, based on the
 * clock and prescale.
 *
 * @todo Support different prescale modes.
 */
void initTimer(int timer, int mode, unsigned long comp);

/**
 * @brief Get the current value of the timer's counter.
 *
 * @param The timer to get the count for.
 * @return TCNTn, the count of the timer. Returns 0 if invalid timer number.
 */
unsigned getTimer(int timer);

/**
 * @brief Initialize PWM for a given timer.
 * @param timer The number of timer to use.
 */
void initPWM(unsigned char timer);

/**
 * @brief Set the PWM on a given timer.
 * For a given timer, this will cause the timer to count up to TOP and then back
 * down again. When counting up, the pin output OCnB (n=timer) will be set high
 * until the timer compares equal to comp, at which point it will be toggled low
 * and then toggled high again when counting down.
 * The PWM signal itself is output on the OCnB pin.
 * @param timer The timer to set the PWM for.
 * @param top The timer value to count up (TOP) before counting down again.
 * @param comp The value at which to toggle OCnB.
 */
void setPWM(unsigned char timer, unsigned int top, unsigned int comp);

/**
 * @brief Only used when the specified timer is in CTC mode. Changes
 * the value of the comparison register to the new specified value.
 *
 * @param timer The timer comparison value to change (0-2).
 * @param comp The value to set the comparison register to.
 *
 * @todo Create a function that will set a new compare value for the given timer.
 */
void setCompValue(unsigned char timer, unsigned long comp);

/**
 * @brief Sets the appropriate interrupt vector to the given callback location.
 *
 * @param timer The timer to set the interrupt on (0-2).
 * @param callback The function callback to set for the interrupt vector.
 *
 * @todo Test.
 */
void setTimerInterrupt(unsigned char timer, Callback callback);

#endif /* TIMER_H_ */
