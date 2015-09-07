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

#endif  // __UTIL_H__
