#include "game.h"

inline tile_type_t tile_type(tile_t tile) {
  return tile & TILE_MASK_TYPE;
}

inline void tile_set_type(tile_t *tile, tile_type_t type) {
  *tile &= ~TILE_MASK_TYPE;
  *tile |= type;
}

inline tile_upgrade_t tile_upgrade(tile_t tile) {
  return (tile & TILE_MASK_UPGRADE);
}

inline void tile_set_upgrade(tile_t *tile, tile_upgrade_t upgrade) {
  *tile &= ~TILE_MASK_UPGRADE;
  *tile |= upgrade;
}

inline bool tile_contains_bomb(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_BOMB;
}

inline void tile_set_contains_bomb(tile_t *tile, bool bomb) {
  *tile &= ~TILE_MASK_CONTAINS_BOMB;
  if (bomb) *tile |= TILE_MASK_CONTAINS_BOMB;
}

inline bool tile_contains_explosion(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_EXPLOSION;
}

inline void tile_set_contains_explosion(tile_t *tile, bool explosion) {
  *tile &= ~TILE_MASK_CONTAINS_EXPLOSION;
  if (explosion) *tile |= TILE_MASK_CONTAINS_EXPLOSION;
}

inline bool tile_contains_player_1(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_PLAYER_1;
}

inline void tile_set_contains_player_1(tile_t *tile, bool player_1) {
  *tile &= ~TILE_MASK_CONTAINS_PLAYER_1;
  if (player_1) *tile |= TILE_MASK_CONTAINS_PLAYER_1;
}

inline bool tile_contains_player_2(tile_t tile) {
  return tile & TILE_MASK_CONTAINS_PLAYER_2;
}

inline void tile_set_contains_player_2(tile_t *tile, bool player_2) {
  *tile &= ~TILE_MASK_CONTAINS_PLAYER_2;
  if (player_2) *tile |= TILE_MASK_CONTAINS_PLAYER_2;
}
