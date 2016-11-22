#include <stdlib.h>
#include "game.h"

static player_t players[PLAYER_COUNT];

void init_players() {
  player_t *p1 = &players[0];
  player_t *p2 = &players[1];

  p1->player_id = 1;
  p1->x = PLAYER_1_DEFAULT_X;
  p1->y = PLAYER_1_DEFAULT_Y;
  p1->lives = PLAYER_DEFAULT_LIVES;
  p1->damage_countdown = 0;
  p1->movement_countdown = 0;
  p1->movement_default_countdown = PLAYER_DEFAULT_MOVEMENT_COUNTDOWN;

  p2->player_id = 2;
  p2->x = PLAYER_2_DEFAULT_X;
  p2->y = PLAYER_2_DEFAULT_Y;
  p2->lives = PLAYER_DEFAULT_LIVES;
  p2->damage_countdown = 0;
  p2->movement_countdown = 0;
  p2->movement_default_countdown = PLAYER_DEFAULT_MOVEMENT_COUNTDOWN;
}

static void update_player(player_t *player) {
  if (!player->lives) {
    return;
  }

  tile_t *tile = world_tile(player->x, player->y);

  if (tile_contains_explosion(*tile) && player->damage_countdown != 0) {
    player->lives--;
    player->damage_countdown = PLAYER_DAMAGE_COUNTDOWN;
  }

  if (player->movement_countdown > 0) player->movement_countdown--;
  if (player->damage_countdown   > 0) player->damage_countdown--;
}

void update_players() {
  update_player(&players[0]);
  update_player(&players[1]);
}
