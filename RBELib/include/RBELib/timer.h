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
/**
 * @brief Timer normal mode.
 */
#define NORMAL 0
/**
 * @brief Timer Clear Timer on Compare (CTC) mode.
 */
#define CTC 2

/**
 * @brief A function pointer which takes no arguments and returns void.
 */
typedef void (*Callback)();

enum PreScale {
  k1=1,
  k1024=5,
};

/**
 * @brief Initializes the specified timer in the specified mode. This
 * header file provides constants for NORMAL operation mode and
 * CTC operation mode, however there are many more modes of operation
 * for the Timers that can be experimented with.
 *
 * @param timer The number of the timer to be initialized (0-2).
 * @param mode The mode to initialize the specified timer in.
 * @param comp Only used in CTC mode. The period, in microseconds, based on the
 * clock and prescale.
 *
 * @todo Create a function that initializes the desired timer in a given mode
 *and set the compare value
 * (as appropriate).
 */
void initTimer(int timer, int mode, unsigned long comp);

unsigned getTimer(int timer);

void initPWM(unsigned char timer);
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
