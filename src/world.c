#include "game.h"

inline tile_type_t tile_type(tile_t tile) {
  return tile & 0b00000011;
}

inline tile_upgrade_t tile_upgrade(tile_t tile) {
  return (tile & 0b00001100) >> 2;
}

inline bool tile_contains_bomb(tile_t tile) {
  return tile & 0b00010000;
}

inline bool tile_contains_explosion(tile_t tile) {
  return tile & 0b00100000;
}

inline bool tile_contains_player_1(tile_t tile) {
  return tile & 0b01000000;
}

inline bool tile_contains_player_2(tile_t tile) {
  return tile & 0b10000000;
}
