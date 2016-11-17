#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

/* World ------------------------------------------------------------------- */

#define TILE_TYPE_EMPTY 0
#define TILE_TYPE_SOLID 1
#define TILE_TYPE_STATIC 2

#define TILE_UPGRADE_NONE 0
#define TILE_UPGRADE_RANGE 1
#define TILE_UPGRADE_BOMBS 2
#define TILE_UPGRADE_SPEED 3

typedef uint8_t tile_t;
typedef uint8_t tile_type_t;
typedef uint8_t tile_upgrade_t;

inline tile_type_t tile_type(tile_t tile);
inline tile_upgrade_t tile_upgrade(tile_t tile);
inline bool tile_contains_bomb(tile_t tile);
inline bool tile_contains_explosion(tile_t tile);
inline bool tile_contains_player_1(tile_t tile);
inline bool tile_contains_player_2(tile_t tile);

// TODO: Tile update functions

typedef struct {
  uint8_t width;
  uint8_t height;
  tile_t tiles[];
} world_t;

/* Player ------------------------------------------------------------------ */

typedef struct {
  uint8_t player_id;
  uint8_t x;
  uint8_t y;
  uint8_t lives;
  uint8_t damage_countdown;
  uint8_t movement_countdown;
  uint8_t movement_default_countdown;
  uint8_t explosion_range;
} player_t;

/* Bomb -------------------------------------------------------------------- */

typedef struct {
  player_t *player;
  uint8_t x;
  uint8_t y;
  uint8_t countdown;
} bomb_t;

void update_bombs();
bomb_t *place_bomb(player_t *player);
void trigger_bomb(bomb_t* bomb);

/* Explosion --------------------------------------------------------------- */

typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t countdown;
} explosion_t;

void update_explosions();
void activate_explosion(uint8_t x, uint8_t y);

#endif /* GAME_H */
