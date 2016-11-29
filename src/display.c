#include "game.h"

void render_cycle() {
  tile_t *tile;
  uint8_t x, y;
  for (y = 0; y < WORLD_HEIGHT; y++) {
    for (x = 0; x < WORLD_WIDTH; x++) {
      tile = world_tile(x, y);
      if (tile_needs_render_update(*tile)) {
        renderer(x, y);
        tile_set_render_update(tile, false);
      }
    }
  }
}

void control_cycle() {
  control_handler();
}
