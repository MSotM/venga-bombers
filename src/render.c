#include "game.h"

void init_render() {
#ifdef RENDER_TERMINAL
  init_terminal_display();
#endif

#ifdef RENDER_LCD
  init_lcd_display();
#endif

  render(true);
}

static void render_to_displays(uint8_t x, uint8_t y) {
#ifdef RENDER_TERMINAL
  render_to_terminal(x, y);
#endif

#ifdef RENDER_LCD
  render_to_lcd(x, y);
#endif
}

void render(bool force) {
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
}
