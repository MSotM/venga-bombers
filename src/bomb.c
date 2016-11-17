#include <stdlib.h>
#include "game.h"

#define BOMB_COUNT 32
#define BOMB_DEFAULT_COUNTDOWN 32

static bomb_t bombs[BOMB_COUNT];
static uint8_t next_bomb_index = 0;

void update_bombs() {
  int i;
  bomb_t *bomb;
  for (i = 0; i < BOMB_COUNT; i++) {
    bomb = &bombs[i];

    if (bomb->player != NULL && --bomb->countdown == 0) {
      trigger_bomb(bomb);
    }
  }
}

bomb_t *place_bomb(player_t *player) {
  bomb_t *bomb = &bombs[next_bomb_index];

  // We assume the bomb is unused, so we can just override its values.
  bomb->player = player;
  bomb->x = player->x;
  bomb->y = player->y;
  bomb->countdown = BOMB_DEFAULT_COUNTDOWN;

  // TODO: Update tiles

  if (++next_bomb_index == BOMB_COUNT) {
    next_bomb_index = 0;
  }

  return bomb;
}

// TODO: trigger_bomb
