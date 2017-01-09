#include <pleasant-usart.h>
#include <pleasant-twi.h>
#include <pleasant-lcd.h>
#include "game.h"
#include "touch-buttons.h"

/* Terminal controls ------------------------------------------------------- */

static bool terminal_controls_init() {
  /* This also happens during terminal renderer initialization. */
  usart_init(115200,
             USART_DEFAULT_ASYNCHRONOUS_MODE,
             USART_DEFAULT_PARITY,
             USART_DEFAULT_STOP_BIT_COUNT,
             USART_DEFAULT_CHARACTER_SIZE);
  return true;
}

static void terminal_game_controls_process() {
  enum usart_error error;
  uint8_t input;
  while (usart_byte_available()) {
    input = usart_read(&error);
    /* TODO handle usart error */
    switch (input) {
    case 'w': queue_event(EVENT_TYPE_MOVE_UP, 1); break;
    case 'a': queue_event(EVENT_TYPE_MOVE_LEFT, 1); break;
    case 's': queue_event(EVENT_TYPE_MOVE_DOWN, 1); break;
    case 'd': queue_event(EVENT_TYPE_MOVE_RIGHT, 1); break;
    case 'i': queue_event(EVENT_TYPE_MOVE_UP, 2); break;
    case 'j': queue_event(EVENT_TYPE_MOVE_LEFT, 2); break;
    case 'k': queue_event(EVENT_TYPE_MOVE_DOWN, 2); break;
    case 'l': queue_event(EVENT_TYPE_MOVE_RIGHT, 2); break;
    case 'q': queue_event(EVENT_TYPE_PLACE_BOMB, 1); break;
    case 'u': queue_event(EVENT_TYPE_PLACE_BOMB, 2); break;
    }
  }
}

static void terminal_menu_controls_process() {
  enum usart_error error;
  uint8_t input;
  while (usart_byte_available()) {
    input = usart_read(&error);
    switch (input) {
    case ' ': switch_state(GAME_STATE_PLAYING); break;
    }
  }
}

static void terminal_end_controls_process() {
  enum usart_error error;
  uint8_t input;
  while (usart_byte_available()) {
    input = usart_read(&error);
    switch (input) {
      case ' ': switch_state(GAME_STATE_PLAYING); break;
      case 'm': switch_state(GAME_STATE_MENU); break;
    }
  }
}

/* Nunchuck controls ------------------------------------------------------- */

#define NUNCHUCK_ADDRESS 0x52

static uint8_t nunchuck_position_x;
static uint8_t nunchuck_position_y;
/* Accelerometer data is ignored. */
static bool nunchuck_button_z;
static bool nunchuck_button_c;

static bool nunchuck_controls_init() {
  bool write_succeeded;
  enum twi_error twi_error;
  uint8_t data[] = { 0xF0, 0x55, 0xFB, 0x00 };

  twi_init();

  write_succeeded = twi_write(NUNCHUCK_ADDRESS,
                              data,
                              sizeof(data),
                              &twi_error);

  return write_succeeded && twi_error == TWI_ERROR_NONE;
}

static bool nunchuck_controls_finish_read() {
  bool write_succeeded;
  enum twi_error twi_error;
  uint8_t data[] = { 0, 0 };

  write_succeeded = twi_write(NUNCHUCK_ADDRESS,
                              data,
                              sizeof(data),
                              &twi_error);

  return write_succeeded && twi_error == TWI_ERROR_NONE;
}

static bool nunchuck_game_controls_process() {
  /* The first read we do results in bogus data, which should be ignored. */
  static bool ignore_read = true;

  uint8_t bytes_read;
  enum twi_error twi_error;
  uint8_t data[6];

  bytes_read = twi_read(NUNCHUCK_ADDRESS, data, sizeof(data), &twi_error);
  if (bytes_read != sizeof(data)
      || twi_error != TWI_ERROR_NONE
      || !nunchuck_controls_finish_read()) {
    return false;
  }

  if (ignore_read) {
    ignore_read = false;
    return true;
  }

  nunchuck_position_x = data[0];
  nunchuck_position_y = data[1];

  nunchuck_button_z = !(data[5] & (1 << 0));
  nunchuck_button_c = !(data[5] & (1 << 1));

  if (nunchuck_position_x < 100) {
    queue_event(EVENT_TYPE_MOVE_LEFT, 1);
  } else if (nunchuck_position_x > 150) {
    queue_event(EVENT_TYPE_MOVE_RIGHT, 1);
  } else if (nunchuck_position_y > 150) {
    queue_event(EVENT_TYPE_MOVE_UP, 1);
  } else if (nunchuck_position_y < 100) {
    queue_event(EVENT_TYPE_MOVE_DOWN, 1);
  }

  if (nunchuck_button_c) {
    queue_event(EVENT_TYPE_PLACE_BOMB, 1);
  }

  return true;
}

/* Touch controls ---------------------------------------------------------- */

bool is_button_pressed(uint16_t touch_x,
                       uint16_t touch_y,
                       touch_button_t button) {
  return touch_x >= button.x && touch_x <= button.x + button.width &&
         touch_y >= button.y && touch_y <= button.y + button.height;
}

touch_button_t menu_buttons[NUM_MENU_BUTTONS];

touch_button_t menu_button_play = {
  .x      = MENU_BUTTON_PLAY_X,
  .y      = MENU_BUTTON_PLAY_Y,
  .width  = DEFAULT_BUTTON_WIDTH,
  .height = DEFAULT_BUTTON_HEIGHT,
  .index  = MENU_BUTTON_PLAY
};

touch_button_t end_buttons[NUM_END_BUTTONS];

touch_button_t end_button_back = {
  .x      = END_BUTTON_BACK_TO_MENU_X,
  .y      = END_BUTTON_BACK_TO_MENU_Y,
  .width  = DEFAULT_BUTTON_WIDTH,
  .height = DEFAULT_BUTTON_HEIGHT,
  .index  = END_BUTTON_BACK_TO_MENU
};

touch_button_t end_button_play = {
  .x      = END_BUTTON_PLAY_AGAIN_X,
  .y      = END_BUTTON_PLAY_AGAIN_Y,
  .width  = DEFAULT_BUTTON_WIDTH,
  .height = DEFAULT_BUTTON_HEIGHT,
  .index  = END_BUTTON_PLAY_AGAIN
};

static bool touch_controls_init() {
  lcd_touch_start_calibration();

  menu_buttons[MENU_BUTTON_PLAY] = menu_button_play;

  end_buttons[END_BUTTON_BACK_TO_MENU] = end_button_back;
  end_buttons[END_BUTTON_PLAY_AGAIN] = end_button_play;

  return true;
}

static uint8_t touch_controls_read(uint16_t touch_x,
                                   uint16_t touch_y,
                                   touch_button_t *buttons,
                                   uint8_t num_buttons) {
  uint8_t button_index;
  for (button_index = 0;
       button_index < num_buttons;
       button_index++) {
    if (is_button_pressed(touch_x, touch_y, buttons[button_index])) {
      return buttons[button_index].index;
    }
  }

  return NO_BUTTON_PRESSED;
}

static void touch_menu_controls_process() {
  uint16_t x, y;

  if (lcd_touch_read(NULL, &x, &y)) {
    touch_menu_button_t button =
      (touch_menu_button_t) touch_controls_read(x,
                                                y,
                                                menu_buttons,
                                                NUM_MENU_BUTTONS);

    switch (button) {
    case MENU_BUTTON_PLAY: switch_state(GAME_STATE_PLAYING); break;

    case NUM_MENU_BUTTONS:
    default:
      break;
    }
  }
}

static void touch_end_controls_process() {
  uint16_t x, y;

  if (lcd_touch_read(NULL, &x, &y)) {
    touch_end_button_t button =
      (touch_end_button_t) touch_controls_read(x,
                                               y,
                                               end_buttons,
                                               NUM_END_BUTTONS);

    switch (button) {
    case END_BUTTON_BACK_TO_MENU: switch_state(GAME_STATE_MENU); break;
    case END_BUTTON_PLAY_AGAIN: switch_state(GAME_STATE_PLAYING); break;

    case NUM_END_BUTTONS:
    default:
      break;
    }
  }
}

/* Controls ---------------------------------------------------------------- */

static bool terminal_controls_enabled = false;
static bool nunchuck_controls_enabled = false;
static bool touch_controls_enabled = false;

void init_controls() {
#ifdef CONTROLS_TERMINAL
  terminal_controls_enabled = terminal_controls_init();
#endif

#ifdef CONTROLS_NUNCHUCK
  nunchuck_controls_enabled = nunchuck_controls_init();
#endif

#ifdef CONTROLS_TOUCH
  touch_controls_enabled = touch_controls_init();
#endif
}

void process_game_controls() {
  if (terminal_controls_enabled) terminal_game_controls_process();
  if (nunchuck_controls_enabled) nunchuck_game_controls_process();
}

void process_menu_controls() {
  if (terminal_controls_enabled) terminal_menu_controls_process();
  if (touch_controls_enabled) touch_menu_controls_process();
}

void process_end_controls() {
  if (terminal_controls_enabled) terminal_end_controls_process();
  if (touch_controls_enabled) touch_end_controls_process();
}
