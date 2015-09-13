/**
 * @file util.h
 *
 * @brief Useful definitions and constants; has things useful to all files.
 *
 * @author James Kuszmaul
 * @date September 6, 2015
 */

#ifndef __UTIL_H__
#define __UTIL_H__

/**
 * @brief A function pointer which takes no arguments and returns void.
 */
typedef void (*Callback)();

/**
 * The CPU clock frequency that we are using.
 */
#define CLK 18432000UL

/**
 * @brief Clip a float to be within a given range.
 */
float clipf(float n, float min, float max);

/**
 * @brief Clip an int to be within a given range.
 */
int clipi(int n, int min, int max);

#endif  // __UTIL_H__
