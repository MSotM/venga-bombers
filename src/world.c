#include "game.h"

inline tile_type_t tile_type(tile_t tile) {
  return tile & TILE_MASK_TYPE;
}

inline tile_upgrade_t tile_upgrade(tile_t tile) {
  return (tile & TILE_MASK_UPGRADE);
}

inline bool tile_contains_bomb(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_BOMB;
}

inline bool tile_contains_explosion(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_EXPLOSION;
}

inline bool tile_contains_player_1(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_PLAYER_1;
}

inline bool tile_contains_player_2(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_PLAYER_2;
}
