#include <stdlib.h>
#include "game.h"

/* World ------------------------------------------------------------------- */

static world_t *world = NULL;

void init_world() {
  world = calloc(sizeof(world_t), 1);
  load_default_world();
  init_players();
}

void update_world() {
  update_explosions();
  update_bombs();
  update_players();
}

tile_t *world_tile(uint8_t x, uint8_t y) {
  if (x >= WORLD_WIDTH || y >= WORLD_HEIGHT) {
    return NULL;
  }

  return &world->tiles[y * WORLD_WIDTH + x];
}

/* Tile accessors ---------------------------------------------------------- */

tile_type_t tile_type(tile_t tile) {
  return tile & TILE_MASK_TYPE;
}

void tile_set_type(tile_t *tile, tile_type_t type) {
  *tile &= ~TILE_MASK_TYPE;
  *tile |= type;
}

tile_upgrade_t tile_upgrade(tile_t tile) {
  return (tile & TILE_MASK_UPGRADE);
}

void tile_set_upgrade(tile_t *tile, tile_upgrade_t upgrade) {
  *tile &= ~TILE_MASK_UPGRADE;
  *tile |= upgrade;
}

bool tile_contains_bomb(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_BOMB;
}

void tile_set_contains_bomb(tile_t *tile, bool bomb) {
  *tile &= ~TILE_MASK_CONTAINS_BOMB;
  if (bomb) *tile |= TILE_MASK_CONTAINS_BOMB;
}

bool tile_contains_explosion(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_EXPLOSION;
}

void tile_set_contains_explosion(tile_t *tile, bool explosion) {
  *tile &= ~TILE_MASK_CONTAINS_EXPLOSION;
  if (explosion) *tile |= TILE_MASK_CONTAINS_EXPLOSION;
}
