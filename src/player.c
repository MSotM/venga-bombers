#include <stdlib.h>
#include "game.h"

static player_t players[PLAYER_COUNT];

player_t *get_player(uint8_t player_id) {
  /* Players should always be stored at the index player_id - 1 */
  return &(players[player_id - 1]);
}

void init_players() {
  player_t *p1 = &(players[0]);
  player_t *p2 = &(players[1]);

  p1->player_id = PLAYER_1_ID;
  p1->lives = PLAYER_DEFAULT_LIVES;
  p1->damage_countdown = 0;
  p1->movement_countdown = 0;
  p1->movement_default_countdown = PLAYER_DEFAULT_MOVEMENT_COUNTDOWN;
  p1->explosion_range = PLAYER_DEFAULT_EXPLOSION_RANGE;
  p1->max_bomb_quantity = PLAYER_DEFAULT_MAX_BOMB_QUANTITY;
  p1->score = 0;
  p1->flags = 0;
  p1->texture = &TEXTURE_PLAYER_1_DOWN;

  p2->player_id = PLAYER_2_ID;
  p2->lives = PLAYER_DEFAULT_LIVES;
  p2->damage_countdown = 0;
  p2->movement_countdown = 0;
  p2->movement_default_countdown = PLAYER_DEFAULT_MOVEMENT_COUNTDOWN;
  p2->explosion_range = PLAYER_DEFAULT_EXPLOSION_RANGE;
  p2->max_bomb_quantity = PLAYER_DEFAULT_MAX_BOMB_QUANTITY;
  p2->score = 0;
  p2->flags = 0;
  p2->texture = &TEXTURE_PLAYER_2_DOWN;
}

static void update_player(player_t *player) {
  if (!player->lives) {
    return;
  }

  tile_t *tile = world_tile(player->x, player->y);

  if (tile_contains_explosion(*tile) && player->damage_countdown == 0) {
    player->lives--;
    set_player_flag(player, PLAYER_FLAG_HEALTH_UPDATED);
    player_add_score(player, -1000);
    if (!player->lives) tile_set_render_update(tile, true);
    player->damage_countdown = PLAYER_DAMAGE_COUNTDOWN;
  }

  switch(tile_upgrade(*tile)) {
  case TILE_UPGRADE_RANGE:
    player->explosion_range++;
    tile_set_upgrade(tile, TILE_UPGRADE_NONE);
    break;
  case TILE_UPGRADE_BOMBS:
    player->max_bomb_quantity++;
    tile_set_upgrade(tile, TILE_UPGRADE_NONE);
    break;
  case TILE_UPGRADE_SPEED:
    /* substract 1 tick if possible */
    if (player->movement_default_countdown >
        PLAYER_MIN_DEFAULT_MOVEMENT_COUNTDOWN) {
      player->movement_default_countdown--;
    }
    tile_set_upgrade(tile, TILE_UPGRADE_NONE);
    break;
  case TILE_UPGRADE_NONE:
  default:
    /* do nothing */
    break;
  }

  if (player->movement_countdown > 0) player->movement_countdown--;
  if (player->damage_countdown   > 0) player->damage_countdown--;
}

void update_players() {
  update_player(&(players[0]));
  update_player(&(players[1]));
}

static const texture_t *player_movement_texture(player_t *player,
                                                int8_t dx,
                                                int8_t dy) {
  if (player->player_id == 1) {
    if (dx < 0) {
      return &TEXTURE_PLAYER_1_LEFT;
    } else if (dx > 0) {
      return &TEXTURE_PLAYER_1_RIGHT;
    } else if (dy < 0) {
      return &TEXTURE_PLAYER_1_UP;
    } else if (dy > 0) {
      return &TEXTURE_PLAYER_1_DOWN;
    }
  } else {
    if (dx < 0) {
      return &TEXTURE_PLAYER_2_LEFT;
    } else if (dx > 0) {
      return &TEXTURE_PLAYER_2_RIGHT;
    } else if (dy < 0) {
      return &TEXTURE_PLAYER_2_UP;
    } else if (dy > 0) {
      return &TEXTURE_PLAYER_2_DOWN;
    }
  }

  return &TEXTURE_PLAYER_1_DOWN;
}

bool player_move(player_t *player, int8_t dx, int8_t dy) {
  tile_t *current_tile, *next_tile;

  if (player->lives == 0)              return false;
  if (player->movement_countdown != 0) return false;

  current_tile = world_tile(player->x, player->y);
  next_tile = world_tile(player->x + dx, player->y + dy);

  player->texture = player_movement_texture(player, dx, dy);
  tile_set_render_update(current_tile, true);

  if (!next_tile)                                return false;
  if (tile_contains_bomb(*next_tile))            return false;
  if (tile_type(*next_tile) == TILE_TYPE_STATIC) return false;
  if (tile_type(*next_tile) == TILE_TYPE_SOLID)  return false;

  tile_set_render_update(next_tile, true);

  player->x = player->x + dx;
  player->y = player->y + dy;
  player->movement_countdown = player->movement_default_countdown;

  return true;
}

void set_player_flag(player_t *player, uint8_t flag) {
  player->flags |= flag;
}

bool get_player_flag(player_t *player, uint8_t flag) {
  return player->flags & flag;
}

void reset_player_flags(player_t *player) {
  player->flags = 0;
}

void reset_all_player_flags() {
  uint8_t player;
  for(player = 0; player < PLAYER_COUNT; player++) {
    reset_player_flags(get_player(player + 1));
  }
}
