#include <avr/interrupt.h>
#include <pleasant-timer.h>
#include "game.h"

volatile bool progress;

uint8_t remote_id = 1;
uint8_t local_id = 2;

ISR(TIMER0_OVF_vect) {
    progress = true;
}

int main() {

  /* Set A2 as input with pullup */
  DDRC &= ~_BV(PORTC2);
  PORTC |= _BV(PORTC2);

  progress = false;
  timer0_init(TIMER_DEFAULT_WAVE_TYPE,
              TIMER_DEFAULT_WRAP_TYPE,
              TIMER_CLOCK_SOURCE_DIV_1024,
              TIMER_INTERRUPT_OVERFLOW,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE);
  sei();


  /* Read dip switch to check if we're player one */
  if(PINC & _BV(PINC2)){
    local_id = 1;
    remote_id = 2;
  }

  init_world();
  init_render();
  init_controls();

  while (true) {
    /* Handle input */
    if (progress) {
      progress = false;
      handle_events();
      update_world();
    }

    render(false);
    process_controls();
  }
}
