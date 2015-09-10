/**
 * @file ADC.h
 *
 * @brief The header file and function prototypes for the ADC.
 * @details For single ended conversion, the ADC value an be found from the voltage using:
 * @f[ \frac {V_{in} * 1024}{V_{ref}} @f]
 * @author Kevin Harrington
 * @date February 11, 2010
 *
 * @author Justin Barrett
 * @date August 23, 2011
 *
 * @author Eric Willcox
 * @date August 19, 2013
 */

#ifndef ADC_H_
#define ADC_H_

#include "RBELib/util.h"

/**
 * @brief Initializes the ADC and make one channel active.
 * You can choose to use either interrupts or polling to read
 * the desired channel.
 *
 * @param channel The ADC channel to initialize.
 * @param free Whether to run in free running mode (0 for not, 1 for yes).
 * @param interrupts Whether or not to enable interrupts for the ADC (0=no,
 * 1=yes).
 *
 * @todo Allow user to select different precisions.
 */
void initADC(int channel, int free, int interrupts);

/**
 * @brief Disables ADC functionality and clears any saved values (globals).
 *
 * @param channel  The ADC channel to disable.
 */
void clearADC(int channel);

/**
 * @brief Run a conversion on and get the analog value from one ADC
 * channel if using polling.
 *
 * This will both chage the ADC channel in ADMUX and turn off any automatic
 * triggers that may have been set on the ADC. Interrupts are not affected.
 *
 * @param channel  The ADC channel to run a conversion on.
 * @return adcVal The 8-10 bit value returned by the ADC
 * conversion.  The precision depends on your settings and
 * how much accuracy you desire.
 *
 * @todo Create the corresponding function to obtain the value of the
 * last calculation if you are using polling.
 */
unsigned short getADC(int channel);

/**
 * @brief Get the value of the ADC register as is, without specifically
 * requesting a new conversion or turning off interrupts, automatic triggering,
 * etc.
 */
unsigned short getADCFree();

/**
 * @brief Change the channel the ADC is sampling if using interrupts.
 *
 * @param channel  The ADC channel to switch to.
 *
 * @todo Create a way to switch ADC channels if you are using interrupts.
 */
void changeADC(int channel);

/**
 * @brief Set the function to be called when an ADC interrupt is triggered.
 * This function does not itself enable ADC interrupts.
 *
 * @param callback The function to call when an ADC interrupt is triggered.
 */
void setADCInterrupt(Callback callback);

#endif /* ADC_H_ */
