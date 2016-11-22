#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

/* Tile -------------------------------------------------------------------- */

#define TILE_MASK_TYPE               0b00000011
#define TILE_MASK_UPGRADE            0b00001100
#define TILE_MASK_CONTAINS_BOMB      0b00010000
#define TILE_MASK_CONTAINS_EXPLOSION 0b00100000
#define TILE_MASK_CONTAINS_PLAYER_1  0b01000000
#define TILE_MASK_CONTAINS_PLAYER_2  0b10000000

typedef enum {
  TILE_TYPE_EMPTY  = 0,
  TILE_TYPE_SOLID  = 1,
  TILE_TYPE_STATIC = 2
} tile_type_t;

typedef enum {
  TILE_UPGRADE_NONE  = (0 << 2),
  TILE_UPGRADE_RANGE = (1 << 2),
  TILE_UPGRADE_BOMBS = (2 << 2),
  TILE_UPGRADE_SPEED = (3 << 2)
} tile_upgrade_t;

typedef uint8_t tile_t;

tile_type_t tile_type(tile_t tile);
void tile_set_type(tile_t *tile, tile_type_t type);
tile_upgrade_t tile_upgrade(tile_t tile);
void tile_set_upgrade(tile_t *tile, tile_upgrade_t upgrade);
bool tile_contains_bomb(tile_t tile);
void tile_set_contains_bomb(tile_t *tile, bool contains_bomb);
bool tile_contains_explosion(tile_t tile);
void tile_set_contains_explosion(tile_t *tile, bool contains_explosion);
bool tile_contains_player_1(tile_t tile);
void tile_set_contains_player_1(tile_t *tile, bool contains_player_1);
bool tile_contains_player_2(tile_t tile);
void tile_set_contains_player_2(tile_t *tile, bool contains_player_2);

/* World ------------------------------------------------------------------- */

#define WORLD_WIDTH  32
#define WORLD_HEIGHT 24

typedef struct {
  tile_t tiles[WORLD_WIDTH * WORLD_HEIGHT];
} world_t;

void update_world();
tile_t *world_tile(uint8_t x, uint8_t y);

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
void trigger_bomb(bomb_t *bomb);

/* Explosion --------------------------------------------------------------- */

typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t countdown;
} explosion_t;

void update_explosions();
void activate_explosion(uint8_t x, uint8_t y);

#endif /* GAME_H */
