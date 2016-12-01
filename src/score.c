#include "game.h"
#include <inttypes.h>
#include <stdlib.h>

void player_add_score(player_t *player, int16_t scoreDiff) {
  int16_t s = player->score;
  s += scoreDiff;
  if (s < 0) {
    s = 0;
  }
  player->score = s;
  set_player_flag(player, PLAYER_FLAG_SCORE_UPDATED);
}
