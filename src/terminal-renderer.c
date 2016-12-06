#include <pleasant-usart.h>
#include <stdio.h>
#include "game.h"

static bool _initialized = false;

void init_terminal_display() {
  usart_init(115200,
             USART_DEFAULT_ASYNCHRONOUS_MODE,
             USART_DEFAULT_PARITY,
             USART_DEFAULT_STOP_BIT_COUNT,
             USART_DEFAULT_CHARACTER_SIZE);

  /* Reset the cursor to the top left position of the terminal */
  usart_write_string("\x1b[H");

  uint8_t i;
  /* Print the top border */
  for (i = 0; i < WORLD_WIDTH + 2; i++) {
    usart_write('=');
  }
  usart_write_string("\x1b[E");

  /* Print the playing field */
  uint8_t x, y;
  for (y = 0; y < WORLD_HEIGHT; y++) {
    usart_write('=');
    for (x = 0; x < WORLD_WIDTH; x++) {
      usart_write(' ');
    }
    usart_write_string("=\x1b[E");
  }

  /* The bottom border */
  for (i = 0; i < WORLD_WIDTH + 2; i++) {
    usart_write('=');
  }
  usart_write_string("\x1b[E");

  /* Render incremental after initialization */
  _initialized = true;
}

const size_t cusror_size = 11;

static void change_cursor(uint8_t x, uint8_t y) {
  char tmp[cusror_size];
  snprintf(tmp, cusror_size, "\x1b[%u;%uH", y + 2, x + 2);
  usart_write_string(tmp);
}

void render_to_terminal(uint8_t x, uint8_t y) {
  if (_initialized) change_cursor(x, y);

  if (get_player(1)->x == x && get_player(1)->y == y && get_player(1)->lives) {
    usart_write_string("\x1b[1;31m$\x1b[0m");
  } else if (get_player(2)->x == x
             && get_player(2)->y == y
             && get_player(2)->lives) {
    usart_write_string("\x1b[1;32m$\x1b[0m");
  } else {
    tile_t *tile = world_tile(x, y);
    switch (tile_type(*tile)) {
    case TILE_TYPE_EMPTY:
      if (tile_contains_bomb(*tile)) {
        usart_write('!');
      } else if (tile_contains_explosion(*tile)) {
        usart_write('X');
      } else {
        tile_upgrade_t upgrade = tile_upgrade(*tile);
        switch(upgrade) {
        case TILE_UPGRADE_SPEED:
          usart_write('&');
          break;
        case TILE_UPGRADE_BOMBS:
          usart_write('%');
          break;
        case TILE_UPGRADE_RANGE:
          usart_write('@');
          break;
        case TILE_UPGRADE_NONE: /* no options left */
          usart_write(' ');
          break;
        }
      }
      break;

    case TILE_TYPE_SOLID:
      usart_write('+');
      break;

    case TILE_TYPE_STATIC:
      usart_write('#');
      break;

    default:
      usart_write('?');
      break;
    }
  }
}
