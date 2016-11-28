#include "game.h"

void load_default_world() {
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

  // Set player positions
  p = get_player(1);
  p->x = 1;
  p->y = 1;
  p = get_player(2);
  p->x = 1;
  p->y = 2;
}