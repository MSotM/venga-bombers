#include "game.h"

static uint64_t random_seed = 4; /* Chosen by fair dice roll.
                                    Guaranteed to be random. */
static uint32_t random_number() {
  random_seed = 6364136223846793005ULL * random_seed + 1;
  return random_seed >> 33;
}

static void place_upgrades() {
  uint8_t x, y;
  tile_t *tile;
  uint8_t random;

  for (x = 0; x < WORLD_WIDTH; x++) {
    for (y = 0; y < WORLD_HEIGHT; y++) {
      tile = world_tile(x, y);

      if (tile_type(*tile) != TILE_TYPE_SOLID) continue;

      random = random_number() % 255;
      if (random < 15)      tile_set_upgrade(tile, TILE_UPGRADE_BOMBS);
      else if (random < 30) tile_set_upgrade(tile, TILE_UPGRADE_SPEED);
      else if (random < 45) tile_set_upgrade(tile, TILE_UPGRADE_RANGE);
    }
  }
}

static void load_default_world() {
  uint8_t x, y;
  player_t *p;

  // Render border
  for (x = 0; x < WORLD_WIDTH; x++) {
    tile_set_type(world_tile(x, 0), TILE_TYPE_STATIC);
    tile_set_type(world_tile(x, WORLD_HEIGHT - 1), TILE_TYPE_STATIC);
  }
  for (y = 1; y < WORLD_HEIGHT - 1; y++) {
    tile_set_type(world_tile(0, y), TILE_TYPE_STATIC);
    tile_set_type(world_tile(WORLD_WIDTH - 1, y), TILE_TYPE_STATIC);
  }

  // Render solids
  for (x = 1; x < WORLD_WIDTH-1; x++) {
    for (y = 1; y < WORLD_HEIGHT-1; y++) {
      tile_set_type(world_tile(x, y), TILE_TYPE_SOLID);
    }
  }

  // Render statics
  for (x = 1; x < (WORLD_WIDTH - 1) / 2; x++) {
    for (y = 1; y < (WORLD_HEIGHT - 1) / 2; y++) {
      tile_set_type(world_tile(x * 2, y*2), TILE_TYPE_STATIC);
    }
  }

  // Clear start positions
  tile_set_type(world_tile(1, 1), TILE_TYPE_EMPTY);
  tile_set_type(world_tile(1, 2), TILE_TYPE_EMPTY);
  tile_set_type(world_tile(2, 1), TILE_TYPE_EMPTY);
  tile_set_type(world_tile(WORLD_WIDTH - 2, WORLD_HEIGHT - 2), TILE_TYPE_EMPTY);
  tile_set_type(world_tile(WORLD_WIDTH - 2, WORLD_HEIGHT - 3), TILE_TYPE_EMPTY);
  tile_set_type(world_tile(WORLD_WIDTH - 3, WORLD_HEIGHT - 2), TILE_TYPE_EMPTY);

  place_upgrades();

  // Set player positions
  p = get_player(1);
  p->x = 1;
  p->y = 1;
  p = get_player(2);
  p->x = WORLD_WIDTH - 2;
  p->y = WORLD_HEIGHT - 2;
}

void load_level(uint8_t number) {
  if(number == 0) {
    load_default_world();
  } else {
    // Load a random level
    // TODO: Implement
  }
}
