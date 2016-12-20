#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <inttypes.h>
#include "options.h"

extern uint8_t remote_id;
extern uint8_t local_id;


/* Tile -----------------------------------------------------------------------
 * Tiles are stored in a single byte, for efficiency. Different bits in the
 * byte signify properties of the tile. These positions can be seen in the
 * various TILE_MASK_* constants. Accessors are provided to get and set the
 * relevant bits in a tile.
 */

typedef uint8_t tile_t;

#define TILE_MASK_TYPE               0b00000011
#define TILE_MASK_UPGRADE            0b00001100
#define TILE_MASK_CONTAINS_BOMB      0b00010000
#define TILE_MASK_CONTAINS_EXPLOSION 0b00100000
#define TILE_MASK_RENDER_UPDATE      0b01000000

/*
 * The type of a tile specifies what kind of place in the world it is.
 * - Empty tiles are tiles that players can move into, place bombs in, etc.
 * - Solid tiles can not be moved through, but an explosion can clear the tile
 *   and make it empty.
 * - Static tiles can not be moved through, and explosions do not affect them.
 */
typedef enum {
  TILE_TYPE_EMPTY  = 0,
  TILE_TYPE_SOLID  = 1,
  TILE_TYPE_STATIC = 2
} tile_type_t;

/*
 * Tiles can contain upgrades, which can be picked up by the player.
 *
 * NOTE: Picking up upgrades is currently not implemented.
 */
typedef enum {
  TILE_UPGRADE_NONE  = (0 << 2),
  TILE_UPGRADE_RANGE = (1 << 2),
  TILE_UPGRADE_BOMBS = (2 << 2),
  TILE_UPGRADE_SPEED = (3 << 2)
} tile_upgrade_t;

tile_type_t tile_type(tile_t tile);
void tile_set_type(tile_t *tile, tile_type_t type);
tile_upgrade_t tile_upgrade(tile_t tile);
void tile_set_upgrade(tile_t *tile, tile_upgrade_t upgrade);
bool tile_contains_bomb(tile_t tile);
void tile_set_contains_bomb(tile_t *tile, bool contains_bomb);
bool tile_contains_explosion(tile_t tile);
void tile_set_contains_explosion(tile_t *tile, bool contains_explosion);
bool tile_needs_render_update(tile_t tile);
void tile_set_render_update(tile_t *tile, bool update);

/* World ----------------------------------------------------------------------
 * The world is the environment every element of the game lives in. It consists
 * of a set of tiles.
 */

#define WORLD_WIDTH  15
#define WORLD_HEIGHT 15

typedef struct {
  tile_t tiles[WORLD_WIDTH * WORLD_HEIGHT];
} world_t;

/*
 * Initialize the global world instance and the players in it.
 */
void init_world();

/*
 * Process another tick in the world. Explosions, bombs and players will
 * progress their timers and perform any relevant activities.
 */
void update_world();

/*
 * Get a pointer to the tile_t at position x,y in the global world. Note that
 * the world will have to be initialized before this point.
 */
tile_t *world_tile(uint8_t x, uint8_t y);

/* Player ---------------------------------------------------------------------
 * Players are user (or AI?)-controlled entities that move around the world,
 * place bombs, pick up upgrades, and are damaged by explosions.
 *
 * Each player is on a tile, but that tile is not marked as containing the
 * player. Players can only be retrieved by ID, using get_player.
 *
 * When the tile a player is on is currently affected by an explosion, one of
 * its lives will be removed and the damage_countdown will be set. Until
 * damage_countdown has reached 0 again, the player can not be damaged again.
 *
 * A player can move into any empty tile, except for ones with a bomb on them.
 * When the player moves, its movement_countdown is reset to its
 * movement_default_countdown. This movement_default_countdown is lowered when
 * a movement speed upgrade is picked up.
 */

#define PLAYER_COUNT                          2
#define PLAYER_1_ID                           1
#define PLAYER_2_ID                           2

#define PLAYER_DAMAGE_COUNTDOWN               32
#define PLAYER_DEFAULT_MOVEMENT_COUNTDOWN     10
#define PLAYER_MIN_DEFAULT_MOVEMENT_COUNTDOWN 2
#define PLAYER_DEFAULT_LIVES                  3
#define PLAYER_DEFAULT_EXPLOSION_RANGE        2
#define PLAYER_DEFAULT_MAX_BOMB_QUANTITY      1

typedef struct {
  uint8_t player_id;
  uint8_t x;
  uint8_t y;
  uint8_t lives;
  uint8_t damage_countdown;
  uint8_t movement_countdown;
  uint8_t movement_default_countdown;
  uint8_t explosion_range;
  uint8_t bombs_placed;
  uint8_t max_bomb_quantity;
  uint16_t score;
  uint8_t flags;
} player_t;

/*
 * Get the player with the specified ID. player_id should be between 1 and
 * PLAYER_COUNT.
 */
player_t *get_player(uint8_t player_id);

/*
 * Initialize the players. Gets called by init_world, so you don't have to.
 */
void init_players();

/*
 * Update the players' countdowns, and handle damage by explosions on the tile
 * the player is currently on. Gets called by update_world, so you don't have
 * to.
 */
void update_players();

/*
 * Move the player, if possible. Returns true if the player moved, or false if
 * not. If the player moved, also sets the player's movement_countdown to its
 * movement_default_countdown.
 */
bool player_move(player_t *player, int8_t dx, int8_t dy);

/* Bomb -----------------------------------------------------------------------
 * Players can place bombs on tiles. Doing so sets the tile_contains_bomb bit,
 * but more information needs to be stored for each bomb. An array is
 * maintained containing pre-allocated bomb_t instances, which will be used to
 * store this additional information.
 *
 * When a bomb explodes, meaning its countdown reaches 0, it will spawn a
 * number of explosions. These explosions will be spawned in straight
 * horizontal and vertical lines, and the amount spawned in each direction
 * depends on the player's explosion_range. When a static tile is encountered
 * along this line, the spreading stops there. When a solid tile is
 * encountered, the spreading continues onto this tile, making it empty, but
 * then stops.
 *
 * A bomb_t whose player property is NULL is not active.
 */

#define BOMB_COUNT 32
#define BOMB_DEFAULT_COUNTDOWN 128

typedef struct {
  player_t *player;
  uint8_t x;
  uint8_t y;
  uint8_t countdown;
} bomb_t;

/*
 * Find a bomb instance based on its location.
 */
bomb_t *get_bomb(uint8_t x, uint8_t y);

/*
 * Update all bombs' countdowns and trigger them if they reach 0. Gets called
 * by update_world, so you don't have to.
 */
void update_bombs();

/*
 * Place a bomb on the player's tile, if there is not yet a bomb there.
 */
bomb_t *place_bomb(player_t *player);

/*
 * Trigger the bomb, placing explosions based on the player's explosion_range.
 */
void trigger_bomb(bomb_t *bomb);

/* Explosion ------------------------------------------------------------------
 * When a bomb explodes, it spawns a number of explosions. This sets the
 * tile_contains_explosion bit, but more information needs to be stored for
 * each explosion. An array is maintained containing pre-allocated explosion_t
 * instances, which will be used to store this additional information.
 *
 * Note that a single tile can never have multiple active explosions on it.
 * Instead, were that to happen, the previous explosion_t will be reused.
 *
 * Also note that explosions, aside from changing the tile_contains_explosion
 * bit of the tile they're on, don't directly affect the world.
 *
 * An explosion_t with a countdown of 0 is not active.
 */

#define EXPLOSION_COUNT 128
#define EXPLOSION_DEFAULT_COUNTDOWN 32

typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t countdown;
} explosion_t;

/*
 * Update all explosions' countdowns. Called by update_world, so you don't have
 * to.
 */
void update_explosions();

/*
 * Place an explosion at the specified position in the world.
 */
void activate_explosion(uint8_t x, uint8_t y);

/* Events ------------------------------------------------------------------ */

typedef enum {
  /* Events in the queue that have already been processed will have their event
     type reset to EVENT_TYPE_PROCESSED. Events of this type are not supposed
     to be queued using queue_event. */
  EVENT_TYPE_PROCESSED,
  EVENT_TYPE_MOVE_UP,
  EVENT_TYPE_MOVE_RIGHT,
  EVENT_TYPE_MOVE_DOWN,
  EVENT_TYPE_MOVE_LEFT,
  EVENT_TYPE_PLACE_BOMB
} event_type_t;

typedef struct {
  event_type_t event_type;
  uint8_t player_id;
} event_t;

/*
 * Place a new event in the event queue.
 */
void queue_event(event_type_t event_type, uint8_t player_id);

/*
 * Remove an event from the event queue. Returns NULL if no events are
 * available. This function should normally not be called by anything but
 * handle_events.
 *
 * Note that after handling the event, it should be freed using free_event.
 */
event_t *dequeue_event();

/*
 * Clean up the event. After calling this, the event should no longer be used.
 */
void free_event(event_t *event);

/* Event handling ---------------------------------------------------------- */

/*
 * Handle all events currently in the event queue, calling appropriate
 * functions in the process. This function stops when all events have been
 * processed. If events are placed in the queue by interrupts faster than
 * handle_events can handle them, this function might block.
 */
void handle_events();

/* Rendering --------------------------------------------------------------- */

/*
 * Initialize rendering and print the first frame.
 */
void init_render();

/*
 * Update all flagged tiles.
 */
void render(bool force);

/*
 * LCD rendering functions used in render.c
 */
void init_lcd_display();
void update_lcd_brightness();
void render_to_lcd(uint8_t x, uint8_t y);
void render_player_to_lcd(player_t *player, bool force);

/*
 * 7 segment rendering functions used in render.c
 */
void init_seven_segment();
void render_player_to_seven_segment(player_t *player);

/*
 * Terminal rendering functions used in render.c
 */
void init_terminal_display();
void render_to_terminal(uint8_t x, uint8_t y);

/* Controls ---------------------------------------------------------------- */

/*
 * Initialize any available input methods.
 */
void init_controls();

/*
 * Process input for all enabled input methods.
 */
void process_controls();

/* Levels ------------------------------------------------------------------ */

/*
 * Load a level (specified by levelNumber) into the world object. The same
 * levelNumber will always yield the same level.
 */
void load_level(uint8_t levelNumber);

/* Score ------------------------------------------------------------------- */

/*
 * Add score to a given player. This can be a positive score (to increment the
 * score of a player), or a negative score (to decrement the score of a
 * player). The score will never go below 0.
 */
void player_add_score(player_t *player, int16_t scoreDiff);

/* Player property flagging ------------------------------------------------ */

#define PLAYER_FLAG_SCORE_UPDATED 1
#define PLAYER_FLAG_HEALTH_UPDATED 2

/*
 * Sets a flag of a player. These are reset after every render.
 */
void set_player_flag(player_t *player, uint8_t flag);

/*
 * Returns a boolean indicating whether the given flag is set on the player.
 */
bool get_player_flag(player_t *player, uint8_t flag);

/*
 * Reset all flags of a player.
 */
void reset_player_flags(player_t *player);

/*
 * Resets all flags of all players. This method should be called after every
 * render.
 */
void reset_all_player_flags();

/* Brightness control ------------------------------------------------------ */

void init_brightness_control();
uint8_t brightness_control_brightness();

#endif /* GAME_H */
