#include <pleasant-usart.h>
#include <stdio.h>
#include "game.h"

static bool _initialized = false;

static void usart_write_integer(int number) {
  uint16_t lastExponent, digit, exponent;
  lastExponent = 0;

  // Print significant digits
  do {
    digit = number;
    exponent = 1;
    lastExponent = 0;
    while (digit > 9) {
      digit /= 10;
      exponent *= 10;
      lastExponent++;
    }
    usart_write(digit + '0');
    number -= exponent * digit;
  } while (number);

  // Print trailing 0's
  for(digit = 0; digit < lastExponent; digit++) {
    usart_write('0');
  }
}

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

  /* Print scores */
  uint8_t playerId;
  for(playerId = 1; playerId <= PLAYER_COUNT; playerId++) {
    usart_write_string("Score player ");
    usart_write_integer(playerId);
    usart_write_string(": 0\n");
  }

  /* Render incremental after initialization */
  _initialized = true;
}

static void change_cursor(uint8_t x, uint8_t y) {
  usart_write_string("\x1b[");
  usart_write_integer(y + 2);
  usart_write(';');
  usart_write_integer(x + 2);
  usart_write('H');
}

void render_to_terminal(uint8_t x, uint8_t y) {
  // Render score
  uint8_t playerId;
  for (playerId = 1; playerId <= PLAYER_COUNT; playerId++) {
    player_t *p = get_player(playerId);
    if (get_player_flag(p, PLAYER_FLAG_SCORE_UPDATED)) {
      usart_write_string("\x1b[s\x1b[17;0H\x1b[");
      usart_write(playerId + '0');
      usart_write_string("BScore player ");
      usart_write_integer(playerId);
      usart_write_string(": ");
      usart_write_integer(p->score);
      usart_write_string("    \x1b[u");
    }
  }

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
