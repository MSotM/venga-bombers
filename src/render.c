#include "game.h"

void init_render() {
#ifdef RENDER_TERMINAL
  init_terminal_display();
#endif

#ifdef RENDER_LCD
  init_lcd_display();
#endif

#ifdef RENDER_SEVEN_SEGMENT
  init_seven_segment();
#endif
}

static void render_to_displays(uint8_t x, uint8_t y) {
#ifdef RENDER_TERMINAL
  render_to_terminal(x, y);
#endif

#ifdef RENDER_LCD
  render_to_lcd(x, y);
#endif
}

static void render_player_to_displays(player_t *player) {
#ifdef RENDER_LCD
  render_player_to_lcd(player, false);
#endif

#ifdef RENDER_SEVEN_SEGMENT
  if (get_player_flag(player, PLAYER_FLAG_HEALTH_UPDATED) && player->player_id == local_id) {
    render_to_seven_segment(player->lives);
  }
#endif
}

void render_player_info(bool force) {
  /* Render player updates */
  uint8_t i;
  player_t *player;
  for(i = 0; i < PLAYER_COUNT; i++) {
    player = get_player(i + 1);

    /* If player is updated or an update is forced, render the player */
    if(force) {
      player->flags = PLAYER_FLAG_SCORE_UPDATED | PLAYER_FLAG_HEALTH_UPDATED;
    }
    if(player->flags) {
      render_player_to_displays(player);
      reset_player_flags(player);
    }
  }
}

void render(bool force) {
#ifdef RENDER_LCD
  update_lcd_brightness();
#endif

  /* Render all tile updates */
  tile_t *tile;
  uint8_t x, y;
  for (y = 0; y < WORLD_HEIGHT; y++) {
    for (x = 0; x < WORLD_WIDTH; x++) {
      tile = world_tile(x, y);
      if (force || tile_needs_render_update(*tile)) {
        render_to_displays(x, y);
        tile_set_render_update(tile, false);
      }
    }
  }

  render_player_info(force);
}
