#include <avr/interrupt.h>
#include <pleasant-timer.h>
#include "timeout.h"
#include "game.h"

uint8_t remote_id = 1;
uint8_t local_id = 2;

int main() {
  /* This boolean indicates whether the game needs to update its events and
   * logic */
  bool progress = false;

  /* Set A2 as input with pullup */
  DDRC &= ~_BV(PORTC2);
  PORTC |= _BV(PORTC2);

  /* Timer zero is responsible for the game tick and timeouts
   * The timer runs on 61Hz, making it suitable for measuring in seconds or
   * half seconds for example */
  timer0_init(TIMER_DEFAULT_WAVE_TYPE,
              TIMER_DEFAULT_WRAP_TYPE,
              TIMER_CLOCK_SOURCE_DIV_1024,
              TIMER_INTERRUPT_OVERFLOW,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE);
  
  /* Read dip switch to check if we're player one */
  if (PINC & _BV(PINC2)){
    local_id = 1;
    remote_id = 2;
  }

  /* This function must be called before initializations. */
  sei();

  /* Initialize needed components */
  timeout_timer_init();
  init_world();
  init_render();
  init_controls();

  /* Set the initial game loop timeout */
  set_timeout(&progress, 1);

  while (true) {
    /* Handle game logic */
    if (progress) {
      /* reset game timeout */
      progress = false;
      set_timeout(&progress, 1);

      /* Process game actions */
      handle_events();
      update_world();
    }

    render(false);
    process_controls();
    /* Timeouts will be cleared on the main thread, because heap_pop is
     * expensive. Though the given boolean will be very accurate */
    clear_elapsed_timeouts();
  }
}
