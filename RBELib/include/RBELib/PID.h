/**
 * @file PID.h
 *
 * @brief The header file for PID constants and calculations.
 * @details Sets the PID constants and calculate the PID value.
 * @author Eric Willcox
 * @date August 17 2013
 * @author James Kuszmaul
 * @date September 10 2015
 */

#ifndef PID_H_
#define PID_H_

/**
 * @brief PID constants.
 * @details Obtain value using: @code pidConsts.Kp_H; @endcode
 * for the value desired.
 *
 * Do not forget to use @code pidConst pidConsts; @endcode
 * in any file you access them in!
 */
typedef struct {
  /**
   * @brief Upper link Kp.
   */
  float Kp_H;
  /**
   * @brief Upper link Ki.
   */
  float Ki_H;
  /**
   * @brief Upper link Kd.
   */
  float Kd_H;
  /**
   * @brief Lower link Kp.
   */
  float Kp_L;
  /**
   * @brief Lower link Ki.
   */
  float Ki_L;
  /**
   * @brief Lower link Kd.
   */
  float Kd_L;
} pidConst;

/**
* @brief Declaration for use in other files.
*
* @todo Again, do not forget to use @code pidConst pidConsts; @endcode
* in any file you access them in!
*/
extern pidConst pidConsts;

/**
 * @brief The different links in the arm.
 */
typedef enum {
  kH, /*!< Higher/farther out link. */
  kL, /*!< Lower/inner link. */
} Link;

/**
 * @brief Sets the Kp, Ki, and Kd values for 1 link.
 * @details to set the values, use the following style
 * @code pidConst.Kp = 1.3; @endcode
 * @param link The link you want to set the values for (kH or kL).
 * @param Kp Proportional value.
 * @param Ki Integral value.
 * @param Kd Derivative value.
 */
void setConst(Link link, float Kp, float Ki, float Kd);

/**
 * @brief Calculate the PID value.
 * @param  link Which link to calculate the error for (Use kH and kL).
 * @param setPoint The desired position of the link.
 * @param actPos The current position of the link.
 */
signed int calcPID(Link link, int setPoint, int actPos);

/**
 * @brief Calculate the Feed-Forward term.
 * @details Calculates the output necessary to keep the specified link in some
 * pre-determined "neutral" state. At its simplist, this will perform a
 * rudimentary gravity compensation. A more sophisticated feed-forward function
 * would also handle motion profiling or the such.
 * @param link Which link to calculate the feed-forward term--Ends up calling a
 * helper function for either link.
 * @param setPoint The current desired setpoint--may or may not be used.
 * @param actPos The current actual position--may or may not be used.
 * @return The voltage to apply to the corresponding motor, in millivolts.
 * @todo Implement
 */
int calcFF(Link link, int setPoint, int actPos);

#endif /* PERIPH_H_ */
