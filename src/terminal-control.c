#include <pleasant-usart.h>
#include "game.h"

enum usart_error error;

uint8_t last_input = 0;

void terminal_control_handler() {
  while (usart_byte_available()) {
    last_input = usart_read(&error);
    /* TODO handle usart error */
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

control_t control_handler = terminal_control_handler;
