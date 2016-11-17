#include "game.h"

#define EXPLOSION_COUNT 128
#define EXPLOSION_DEFAULT_COUNTDOWN 16

static explosion_t explosions[EXPLOSION_COUNT];
static uint8_t next_explosion_index = 0;

void update_explosions() {
  int i;
  explosion_t *explosion;
  for (i = 0; i < EXPLOSION_COUNT; i++) {
    explosion = &explosions[i];

    if (explosion->countdown != 0) {
      explosion->countdown--;

      // TODO: Update tile
    }
  }
}

void activate_explosion(uint8_t x, uint8_t y) {
  explosion_t *explosion = &explosions[next_explosion_index];

  explosion->x = x;
  explosion->y = y;
  explosion->countdown = EXPLOSION_DEFAULT_COUNTDOWN;

  // TODO: Update tile

  if (++next_explosion_index == EXPLOSION_COUNT) {
    next_explosion_index = 0;
  }
}
