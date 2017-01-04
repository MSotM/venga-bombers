#include <avr/interrupt.h>
#include "timeout.h"

ISR(TIMER0_OVF_vect) {
  /* O(N): this function will process the heap. */
  timeout_timer_interrupt();
}
