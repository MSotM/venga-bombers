#include <pleasant-lcd.h>
#include <stdio.h>
#include "game.h"
#include "font.h"
#include "texture.h"

#define LCD_SQUARE_SIZE                16

#define LCD_UI_X                       240
#define LCD_UI_Y                       0
#define LCD_UI_PLAYER_1_OFFSET_X       0
#define LCD_UI_PLAYER_1_OFFSET_Y       0
#define LCD_UI_PLAYER_2_OFFSET_X       0
#define LCD_UI_PLAYER_2_OFFSET_Y       120

#define LCD_UI_AVATAR_OFFSET_X         16
#define LCD_UI_AVATAR_OFFSET_Y         16

#define LCD_UI_HEALTH_BAR_OFFSET_X     16
#define LCD_UI_HEALTH_BAR_OFFSET_Y     69
#define LCD_UI_HEALTH_BAR_WIDTH        48
#define LCD_UI_HEALTH_BAR_HEIGHT       10
#define LCD_UI_HEALTH_BAR_COLOR_1      RGB(56,  148, 56)
#define LCD_UI_HEALTH_BAR_COLOR_2      RGB(148, 56,  56)

#define LCD_UI_SCORE_OFFSET_X          8
#define LCD_UI_SCORE_OFFSET_Y          85
#define LCD_UI_SCORE_WIDTH             66
#define LCD_UI_SCORE_HEIGHT            16
#define LCD_UI_COLOR_SCORE             RGB(0,   0,   0)
#define LCD_UI_COLOR_SCORE_BACKGROUND  RGB(162, 152, 105)

void init_lcd_display() {
  lcd_init(LCD_DEFAULT_SPI_CLOCK_SPEED);
  init_brightness_control();

  render_player_to_lcd(get_player(1), true);
  render_player_to_lcd(get_player(2), true);
}

static void render_player_status(player_t *player,
                                 uint8_t x_offset,
                                 uint8_t y_offset,
                                 bool force) {
  uint8_t health_bar_filled_width;
  uint8_t health_bar_empty_width;

  if (force) {
    texture_render(TEXTURE_UI, x_offset, y_offset, 1);

    texture_render((player->player_id == PLAYER_1_ID
                    ? TEXTURE_PLAYER_1
                    : TEXTURE_PLAYER_2),
                   x_offset + LCD_UI_AVATAR_OFFSET_X,
                   y_offset + LCD_UI_AVATAR_OFFSET_Y,
                   3);
  }

  if (get_player_flag(player, PLAYER_FLAG_SCORE_UPDATED)) {
    lcd_render_integer(x_offset + LCD_UI_SCORE_OFFSET_X,
                       y_offset + LCD_UI_SCORE_OFFSET_Y,
                       player->score,
                       LCD_UI_COLOR_SCORE,
                       LCD_UI_COLOR_SCORE_BACKGROUND,
                       2,
                       LCD_UI_SCORE_WIDTH);
  }

  if (get_player_flag(player, PLAYER_FLAG_HEALTH_UPDATED)) {
    health_bar_filled_width = (player->lives
                               * LCD_UI_HEALTH_BAR_WIDTH
                               / PLAYER_DEFAULT_LIVES);
    health_bar_empty_width = LCD_UI_HEALTH_BAR_WIDTH - health_bar_filled_width;

    lcd_fill_rect(x_offset + LCD_UI_HEALTH_BAR_OFFSET_X,
                  y_offset + LCD_UI_HEALTH_BAR_OFFSET_Y,
                  health_bar_filled_width,
                  LCD_UI_HEALTH_BAR_HEIGHT,
                  LCD_UI_HEALTH_BAR_COLOR_1);
    lcd_fill_rect((x_offset
                   + LCD_UI_HEALTH_BAR_OFFSET_X
                   + health_bar_filled_width),
                  y_offset + LCD_UI_HEALTH_BAR_OFFSET_Y,
                  health_bar_empty_width,
                  LCD_UI_HEALTH_BAR_HEIGHT,
                  LCD_UI_HEALTH_BAR_COLOR_2);
  }
}

void render_player_to_lcd(player_t *player, bool force) {
  if (player->player_id == 1) {
    render_player_status(player,
                         LCD_UI_X + LCD_UI_PLAYER_1_OFFSET_X,
                         LCD_UI_PLAYER_1_OFFSET_Y,
                         force);
  } else if (player->player_id == 2) {
    render_player_status(player,
                         LCD_UI_X + LCD_UI_PLAYER_2_OFFSET_X,
                         LCD_UI_PLAYER_2_OFFSET_Y,
                         force);
  }
}

void render_to_lcd(uint8_t x, uint8_t y) {
  uint8_t i;
  player_t *player;
  for (i = 0; i < PLAYER_COUNT; i++) {
    player = get_player(i + 1);
    if (player->x == x && player->y == y && player->lives) {
      texture_render((i == 0 ? TEXTURE_PLAYER_1 : TEXTURE_PLAYER_2),
                     x * LCD_SQUARE_SIZE,
                     y * LCD_SQUARE_SIZE,
                     1);
      return;
    }
  }

  tile_t *tile = world_tile(x, y);
  switch (tile_type(*tile)) {
  case TILE_TYPE_EMPTY:
    if (tile_contains_bomb(*tile)) {
      texture_render((get_bomb(x, y)->player->player_id == 1
                      ? TEXTURE_BOMB_PLAYER_1
                      : TEXTURE_BOMB_PLAYER_2),
                     x * LCD_SQUARE_SIZE,
                     y * LCD_SQUARE_SIZE,
                     1);
    } else if (tile_contains_explosion(*tile)) {
      texture_render(TEXTURE_EXPLOSION,
                     x * LCD_SQUARE_SIZE,
                     y * LCD_SQUARE_SIZE,
                     1);
    } else {
      tile_upgrade_t upgrade = tile_upgrade(*tile);
      switch (upgrade) {
      case TILE_UPGRADE_SPEED:
        texture_render(TEXTURE_UPGRADE_SPEED,
                       x * LCD_SQUARE_SIZE,
                       y * LCD_SQUARE_SIZE,
                       1);
        break;
      case TILE_UPGRADE_BOMBS:
        texture_render(TEXTURE_UPGRADE_BOMBS,
                       x * LCD_SQUARE_SIZE,
                       y * LCD_SQUARE_SIZE,
                       1);
        break;
      case TILE_UPGRADE_RANGE:
        texture_render(TEXTURE_UPGRADE_RANGE,
                       x * LCD_SQUARE_SIZE,
                       y * LCD_SQUARE_SIZE,
                       1);
        break;
      case TILE_UPGRADE_NONE: /* no options left */
        texture_render(TEXTURE_EMPTY,
                       x * LCD_SQUARE_SIZE,
                       y * LCD_SQUARE_SIZE,
                       1);
        break;
      }
    }
    break;

  case TILE_TYPE_SOLID:
    texture_render(TEXTURE_SOLID,
                   x * LCD_SQUARE_SIZE,
                   y * LCD_SQUARE_SIZE,
                   1);
    break;

  case TILE_TYPE_STATIC:
    texture_render(TEXTURE_STATIC,
                   x * LCD_SQUARE_SIZE,
                   y * LCD_SQUARE_SIZE,
                   1);
    break;

  default:
    texture_render(TEXTURE_ERROR,
                   x * LCD_SQUARE_SIZE,
                   y * LCD_SQUARE_SIZE,
                   1);
    break;
  }
}

void update_lcd_brightness() {
  lcd_set_brightness(brightness_control_brightness());
}
