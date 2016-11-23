#include "game.h"

void handle_events() {
  player_t *player;
  event_t *event;
  while ((event = dequeue_event())) {
    player = get_player(event->player_id);

    switch(event->event_type) {
    case EVENT_TYPE_MOVE_UP:    player_move(player,  0, -1); break;
    case EVENT_TYPE_MOVE_RIGHT: player_move(player,  1,  0); break;
    case EVENT_TYPE_MOVE_DOWN:  player_move(player,  0,  1); break;
    case EVENT_TYPE_MOVE_LEFT:  player_move(player, -1,  0); break;
    case EVENT_TYPE_PLACE_BOMB: place_bomb(player);          break;
    case EVENT_TYPE_PROCESSED:                               break;
    }

    free_event(event);
  }
}
