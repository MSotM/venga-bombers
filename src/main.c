#include <avr/interrupt.h>
#include <pleasant-timer.h>
#include "timeout.h"
#include "game.h"
#include "audio.h"

uint8_t local_id = 1;
uint8_t remote_id = 2;

int main() {
  pre_init_music();

  /* This boolean indicates whether the game needs to update its events and
   * logic */
  bool progress = false;

  /* Set A2 as input with pullup */
  DDRC &= ~_BV(PORTC2);
  PORTC |= _BV(PORTC2);

  /* Timer zero is responsible for generating tones. It does not have a
   * constant prescaler making it unsuitable for measuring certain units.
   * Thus this timer cannot be reused for any other activities. */
  timer0_init(TIMER_WAVE_TYPE_NORMAL,
              TIMER_DEFAULT_WRAP_TYPE,
              TIMER_CLOCK_SOURCE_DIV_1024,
              TIMER_INTERRUPT_COMPARE_B,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE,
              TIMER_COMPARE_OUTPUT_MODE_TOGGLE);

  /* Timer two is responsible for the timeouts and infra red.
   * The timer runs on 61Hz, making it suitable for measuring in seconds or
   * half seconds for example */
  timer2_init(TIMER_WAVE_TYPE_NORMAL,
              TIMER_WRAP_TYPE_COMPARE_A,
              TIMER_CLOCK_SOURCE_DIV_1024,
              TIMER_INTERRUPT_COMPARE_A,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE,
              TIMER_DEFAULT_COMPARE_OUTPUT_MODE);

  /* Read dip switch to check if we're player one */
  if (PINC & _BV(PINC2)){
    local_id = 2;
    remote_id = 1;
  }

  TIMER2_COMPARE_A = 49;

  /* This function must be called before initializations. */
  sei();

  /* Initialize needed components */
  timeout_timer_init();
  init_render();
  init_controls();
  post_init_music();
  /* Set the initial game loop timeout */
  set_timeout(&progress, 40);

  switch_state(GAME_STATE_MENU);

  while (true) {
    /* Handle game logic */
    if (progress) {
      /* reset game timeout */
      progress = false;
      set_timeout(&progress, 40);

      /* Process game actions */
      execute_current_state();
      handle_events();
    }

    conduct();

    /* Timeouts will be cleared on the main thread, because heap_pop is
     * expensive. Though the given boolean will be very accurate */
    clear_elapsed_timeouts();
  }
}
