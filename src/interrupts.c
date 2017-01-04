#include <avr/interrupt.h>
#include "timeout.h"


ISR(TIMER0_COMPB_vect) {
  /* There is no wrap type for B, thus manually */
  TCNT0 = 0;
}

ISR(TIMER2_COMPA_vect) {
  /* O(N): this function will process the heap. */
  timeout_timer_interrupt();
}
