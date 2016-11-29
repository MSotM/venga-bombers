#include <stdlib.h>
#include "game.h"

static explosion_t explosions[EXPLOSION_COUNT];
static uint8_t next_explosion_index = 0;

static explosion_t *get_explosion(uint8_t x, uint8_t y) {
  size_t i;
  explosion_t *explosion;
  for (i = 0; i < EXPLOSION_COUNT; i++) {
    explosion = &(explosions[i]);

    if (explosion->x == x && explosion->y == y) {
      if (explosion->countdown) {
        return explosion;
      }
      return NULL;
    }
  }

  return NULL;
}

void update_explosions() {
  int i;
  explosion_t *explosion;
  tile_t *tile;
  for (i = 0; i < EXPLOSION_COUNT; i++) {
    explosion = &(explosions[i]);

    if (explosion->countdown != 0 && --explosion->countdown == 0) {
      tile = world_tile(explosion->x, explosion->y);
      tile_set_contains_explosion(tile, false);
      tile_set_render_update(tile, true);
    }
  }
}

void activate_explosion(uint8_t x, uint8_t y) {
  explosion_t *explosion;
  tile_t *tile = world_tile(x, y);

  /* An explosion may already be active at this location, in which case
     creating another one would not be particularly useful. The end of the
     countdown of that other explosion would also cause the tile to no longer
     be marked as containing an explosion. We can just reuse the other
     explosion. */
  if (tile_contains_explosion(*tile)) {
    explosion = get_explosion(x, y);
  } else {
    explosion = &(explosions[next_explosion_index]);
    if (++next_explosion_index == EXPLOSION_COUNT) {
      next_explosion_index = 0;
    }
  }

  explosion->x = x;
  explosion->y = y;
  explosion->countdown = EXPLOSION_DEFAULT_COUNTDOWN;

  tile_set_contains_explosion(world_tile(x, y), true);
  tile_set_render_update(tile, true);
}
