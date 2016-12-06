#include <pleasant-usart.h>
#include <pleasant-twi.h>
#include "game.h"

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

static void terminal_controls_process() {
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

static bool nunchuck_controls_process() {
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

/* Controls ---------------------------------------------------------------- */

static bool terminal_controls_enabled = false;
static bool nunchuck_controls_enabled = false;

void init_controls() {
#ifdef CONTROLS_TERMINAL
  terminal_controls_enabled = terminal_controls_init();
#endif

#ifdef CONTROLS_NUNCHUCK
  nunchuck_controls_enabled = nunchuck_controls_init();
#endif
}

void process_controls() {
  if (terminal_controls_enabled) terminal_controls_process();
  if (nunchuck_controls_enabled) nunchuck_controls_process();
}
