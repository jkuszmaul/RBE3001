#ifndef __TRACK_H__
#define __TRACK_H__

typedef struct {
  unsigned pos; // The side-to-side position, in mm, of the weight.
  unsigned long time; // The time-of-arrival at the arm's position.
  char real;
} Weight;

/**
 * When regularly called, this returns the time and position
 * of the next weight that the user will need to worry about.
 * In addition to the calculations, this blocks for the time
 * it takes to do two ADC conversions.
 */
Weight updateAndGet();


/**
 * Checks either the far or near IR sensor and updates the positions of
 * the weights appropriately.
 * Should be called very regularly for both IR sensors.
 * @param far 0=near, 1=far, other=seg faults.
 */
void monitor(char far);

/**
 * Records the existence of a weight and places it on the end of the weight
 * queue.
 * @pos The position side-to-side on the conveyor.
 * @time The time-of-arrival at the robot arm.
 */
void registerWeight(unsigned pos, unsigned long time);

/**
 * Remove weight off start of queue.
 */
void removeWeight();

/**
 * Checks to see if any weights have times before the current time and removes
 * them. Allows a tenth second grace period so that the weight's arrival time
 * is really compared to the current time + 0.1s.
 */
void cleanQueue();

/**
 * Get information on next arrival.
 */
Weight nextWeight();

/**
 * Finds the weight (in the queue) closest to the time of update and combine the
 * two. The result is stored in the queue.
 * Currently, "combine" means that the result's time is that of update, while
 * the position is the average of the two.
 */
void updateWeight(unsigned pos, unsigned long time);

#endif  //  __TRACK_H__
