#include <avr/interrupt.h>
#include <pleasant-timer.h>
#include "game.h"

volatile bool progress;

ISR(TIMER0_OVF_vect) {
    progress = true;
}

int main() {
  progress = false;
  timer0_init(TIMER_DEFAULT_WAVE_TYPE,
              TIMER_DEFAULT_WRAP_TYPE,
              TIMER_CLOCK_SOURCE_DIV_1024,
              TIMER_INTERRUPT_OVERFLOW,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE);
  sei();

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
