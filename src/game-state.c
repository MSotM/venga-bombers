#include <pleasant-lcd.h>
#include <pleasant-usart.h>
#include "game.h"

/* STATEMACHINE ------------------------------------------------------------ */

typedef void state_func_t();

state_func_t enter_menu,       execute_menu,       exit_menu,
             enter_playing,    execute_playing,    exit_playing,
             enter_end,        execute_end,        exit_end,
             enter_highscores, execute_highscores, exit_highscores;

state_func_t *enter_state[NUM_GAME_STATES] = {
        enter_menu,
        enter_playing,
        enter_end,
        enter_highscores
};

state_func_t *execute_state[NUM_GAME_STATES] = {
        execute_menu,
        execute_playing,
        execute_end,
        execute_highscores
};

state_func_t *exit_state[NUM_GAME_STATES] = {
        exit_menu,
        exit_playing,
        exit_end,
        exit_highscores
};

game_state_t current_game_state = NO_STATE_ENTERED;

void switch_state(game_state_t new_game_state) {
  if (current_game_state != NO_STATE_ENTERED) {
    exit_state[current_game_state]();
  }
  enter_state[new_game_state]();
  current_game_state = new_game_state;
}

void execute_current_state() {
  if (current_game_state != NO_STATE_ENTERED) {
    execute_state[current_game_state]();
  }
}

/* MENU -------------------------------------------------------------------- */

void enter_menu() {
  render_menu_background_to_lcd();
  render_menu_to_lcd();
}

void execute_menu() {
  process_menu_controls();
}

void exit_menu() {

}

/* PLAYING ----------------------------------------------------------------- */

void enter_playing() {
  init_world();
  render_player_to_lcd(get_player(1), true);
  render_player_to_lcd(get_player(2), true);
  render(true); /* Initial field render */
}

void execute_playing() {
  update_world();
  render(false);
  process_game_controls();
}

void exit_playing() {

}

/* END --------------------------------------------------------------------- */

void enter_end() {

}

void execute_end() {

}

void exit_end() {

}

/* HIGHSCORES -------------------------------------------------------------- */

void enter_highscores() {

}

void execute_highscores() {

}

void exit_highscores() {

}
