#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <stdbool.h>

/*
 * Initializes the timer interrupt for measuring in 60Hz
 */
void timeout_timer_init();

/*
 * Timer function called by interrupts.c
 * Do not call this function from the main code.
 */
void timeout_timer_interrupt();

/*
 * Give a (preferred global) boolean which will be set to true when the
 * timeout exceeded.
 */
void set_timeout(bool *fired, int timeout);

/*
 * Clear all timeouts which are triggered. This will not be called by the
 * timer because of the expensive pop function of the heap.
 */
void clear_elapsed_timeouts();

#endif /* TIMEOUT_H */
