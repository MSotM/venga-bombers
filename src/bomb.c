#include <stdlib.h>
#include "game.h"

static bomb_t bombs[BOMB_COUNT];
static uint8_t next_bomb_index = 0;

void update_bombs() {
  int i;
  bomb_t *bomb;
  for (i = 0; i < BOMB_COUNT; i++) {
    bomb = &(bombs[i]);

    /* Only use bombs that are active */
    if (bomb->player != NULL) {
      /* If bomb is on explosion, trigger the bomb */
      tile_t *tile = world_tile(bomb->x, bomb->y);
      if (tile_contains_explosion(*tile)) {
        trigger_bomb(bomb);
        continue;
      }
      /* If countdown is 0, trigger the bomb */
      if (--bomb->countdown == 0) {
        trigger_bomb(bomb);
      }
    }
  }
}

bomb_t *place_bomb(player_t *player) {
  bomb_t *bomb;
  tile_t *tile = world_tile(player->x, player->y);

  if (tile_contains_bomb(*tile) || !player->lives) {
    return NULL;
  }

  if (player->bombs_placed >= player->max_bomb_quantity) {
    return NULL;
  }

  bomb = &(bombs[next_bomb_index]);

  /* We assume the bomb is unused, so we can just override its values. */
  bomb->player = player;
  bomb->x = player->x;
  bomb->y = player->y;
  bomb->countdown = BOMB_DEFAULT_COUNTDOWN;

  player->bombs_placed++;
  tile_set_contains_bomb(tile, true);

  if (++next_bomb_index == BOMB_COUNT) {
    next_bomb_index = 0;
  }

  return bomb;
}

static void activate_explosion_line(uint8_t x,
                                    uint8_t y,
                                    int8_t dx,
                                    int8_t dy,
                                    uint8_t range) {
  size_t i;
  tile_t *tile;
  for (i = 1; i <= range; i++) {
    x += dx;
    y += dy;
    tile = world_tile(x, y);

    if (!tile) {
      break;
    } else if (tile_type(*tile) == TILE_TYPE_STATIC) {
      break;
    } else if (tile_type(*tile) == TILE_TYPE_SOLID) {
      tile_set_type(tile, TILE_TYPE_EMPTY);
      activate_explosion(x, y);
      break;
    } else {
      activate_explosion(x, y);
    }
  }
}

void trigger_bomb(bomb_t *bomb) {
  uint8_t x = bomb->x;
  uint8_t y = bomb->y;

  bomb->player->bombs_placed--;
  tile_set_contains_bomb(world_tile(bomb->x, bomb->y), false);

  activate_explosion(x, y);
  activate_explosion_line(x, y,  0,  1, bomb->player->explosion_range);
  activate_explosion_line(x, y,  0, -1, bomb->player->explosion_range);
  activate_explosion_line(x, y,  1,  0, bomb->player->explosion_range);
  activate_explosion_line(x, y, -1,  0, bomb->player->explosion_range);

  player_add_score(bomb->player, 100);

  bomb->player = NULL;
  bomb->x = 0;
  bomb->y = 0;
  bomb->countdown = 0;
}
