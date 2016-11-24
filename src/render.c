#include "game.h"
#include "pleasant-usart.h"

enum usart_error error;

/* Variables to print */
uint8_t last_input = 0;

void init_render() {
  usart_init(115200,
             USART_DEFAULT_ASYNCHRONOUS_MODE,
             USART_DEFAULT_PARITY,
             USART_DEFAULT_STOP_BIT_COUNT,
             USART_DEFAULT_CHARACTER_SIZE);
}

static void field_print(uint8_t x, uint8_t y) {
  if (get_player(1)->x == x && get_player(1)->y == y) {
    usart_write_string("1");
  } else if (get_player(2)->x == x && get_player(2)->y == y) {
    usart_write_string("2");
  } else {
    tile_t *tile = world_tile(x, y);
    switch (tile_type(*tile)) {
    case TILE_TYPE_EMPTY:
      if (tile_contains_bomb(*tile)) {
        usart_write_string("!");
      } else if (tile_contains_explosion(*tile)) {
        usart_write_string("X");
      } else {
        usart_write_string(" ");
      }
      break;

    case TILE_TYPE_SOLID:
      usart_write_string("+");
      break;

    case TILE_TYPE_STATIC:
      usart_write_string("#");
      break;

    default:
      usart_write_string("?");
      break;
    }
  }
}

void render() {
  /* Reset the cursor to the top left position of the terminal */
  usart_write_string("\x1b[0;0H");

  /* Print the top border */
  for (uint8_t i = 0; i < WORLD_WIDTH + 2; i++) {
    usart_write_string("=");
  }
  usart_write_string("\x1b[K\n");

  /* Print the playing field */
  for (uint8_t y = 0; y < WORLD_HEIGHT; y++) {
    usart_write_string("=");
    for (uint8_t x = 0; x < WORLD_WIDTH; x++) {
      field_print(x,y);
    }
    usart_write_string("=\x1b[K\n");
  }

  /* The bottom border */
  for (uint8_t i = 0; i < WORLD_WIDTH + 2; i++) {
    usart_write_string("=");
  }
  usart_write_string("\x1b[K\n");

  /* Print values that are monitored */
  usart_write_string("Values:\x1b[K\n");

  usart_write_string("    Last input: ");
  usart_write(last_input);
  usart_write_string("\x1b[K\n");

  /* Read input and act accordingly */
  while (usart_byte_available()) {
    last_input = usart_read(&error);
    switch (last_input) {
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
