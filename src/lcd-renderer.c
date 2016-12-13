#include <pleasant-lcd.h>
#include <stdio.h>
#include "game.h"
#include "font.h"

#define LCD_SQUARE_SIZE                16

#define LCD_COLOR_PLAYER1              RGB(0,   255, 0  )
#define LCD_COLOR_PLAYER2              RGB(0,   0,   255)
#define LCD_COLOR_BOMB                 RGB(255, 0,   0  )
#define LCD_COLOR_EXPLOSION            RGB(255, 128, 0  )
#define LCD_COLOR_SOLID                RGB(128, 128, 128)
#define LCD_COLOR_STATIC               RGB(0,   0,   0  )
#define LCD_COLOR_UPGRADE_SPEED        RGB(0,   255, 255)
#define LCD_COLOR_UPGRADE_BOMBS        RGB(0,   255, 255)
#define LCD_COLOR_UPGRADE_RANGE        RGB(0,   255, 255)
#define LCD_COLOR_UPGRADE_NONE         RGB(255, 255, 255)
#define LCD_COLOR_UNKNOWN              RGB(255, 0,   255)

#define LCD_UI_COLOR_BORDER            RGB(0,   0,   0)
#define LCD_UI_COLOR_BACKGROUND        RGB(200, 200, 200)
#define LCD_UI_BORDER_WIDTH            8

#define LCD_UI_HEALTH_BAR_OFFSET_Y     70
#define LCD_UI_HEALTH_BAR_HEIGHT       12
#define LCD_UI_HEALTH_BAR_COLOR_1      RGB(0,   255, 0)
#define LCD_UI_HEALTH_BAR_COLOR_2      RGB(255, 0,   0)
#define LCD_UI_HEALTH_BAR_BORDER_COLOR RGB(0,   0,   0)
#define LCD_UI_HEALTH_BAR_BORDER_WIDTH 3

#define LCD_UI_COLOR_SCORE             RGB(0,   0,   0)
#define LCD_UI_SCORE_OFFSET_X          5
#define LCD_UI_SCORE_OFFSET_Y          90

#define LCD_UI_AVATAR_BORDER_WIDTH     3
#define LCD_UI_AVATAR_WIDTH            40
#define LCD_UI_AVATAR_HEIGHT           40
#define LCD_UI_AVATAR_OFFSET_Y         10

static lcd_color get_player_avatar(player_t *player) {
  switch (player->player_id) {
  case PLAYER_1_ID:
    return LCD_COLOR_PLAYER1;
  case PLAYER_2_ID:
    return LCD_COLOR_PLAYER2;
  default:
    return RGB(255, 255, 0);
  }
}

void init_lcd_display() {
  lcd_init(LCD_DEFAULT_SPI_CLOCK_SPEED);
  init_brightness_control();

  /* Print UI background */
  lcd_fill_rect(240 + LCD_UI_BORDER_WIDTH, LCD_UI_BORDER_WIDTH,
                80 - LCD_UI_BORDER_WIDTH * 2, 240 - LCD_UI_BORDER_WIDTH * 2,
                LCD_UI_COLOR_BACKGROUND);

  /* Print horizontal seperation */
  lcd_fill_rect(240 + LCD_UI_BORDER_WIDTH, 120 - LCD_UI_BORDER_WIDTH / 2,
                80 - LCD_UI_BORDER_WIDTH * 2, LCD_UI_BORDER_WIDTH,
                LCD_UI_COLOR_BORDER);

  /* Print left and right border */
  lcd_fill_rect(240, 0, LCD_UI_BORDER_WIDTH, 240, LCD_UI_COLOR_BORDER);
  lcd_fill_rect(320 - LCD_UI_BORDER_WIDTH, 0, LCD_UI_BORDER_WIDTH, 240,
                LCD_UI_COLOR_BORDER);

  /* Print top and bottom border */
  lcd_fill_rect(240 + LCD_UI_BORDER_WIDTH, 0, 80 - LCD_UI_BORDER_WIDTH * 2,
                LCD_UI_BORDER_WIDTH, LCD_UI_COLOR_BORDER);
  lcd_fill_rect(240 + LCD_UI_BORDER_WIDTH, 240 - LCD_UI_BORDER_WIDTH,
                80 - LCD_UI_BORDER_WIDTH * 2, LCD_UI_BORDER_WIDTH,
                LCD_UI_COLOR_BORDER);

  /* Print player info */
  uint8_t i;
  player_t *player;
  for (i = 0; i < PLAYER_COUNT; i++) {
    player = get_player(i + 1);

    /* Determine top and left of player ui */
    uint16_t top = LCD_UI_BORDER_WIDTH + (120 - LCD_UI_BORDER_WIDTH / 2) * i;
    uint16_t left = 240 + LCD_UI_BORDER_WIDTH;

    /* Print health bar */
    lcd_fill_rect(left, top + LCD_UI_HEALTH_BAR_OFFSET_Y,
                  80 - LCD_UI_BORDER_WIDTH * 2, LCD_UI_HEALTH_BAR_HEIGHT,
                  LCD_UI_HEALTH_BAR_BORDER_COLOR);

    /* Set top and left to avatar box */
    top += LCD_UI_AVATAR_OFFSET_Y;
    left += (80 - LCD_UI_AVATAR_WIDTH) / 2 - LCD_UI_BORDER_WIDTH -
            LCD_UI_AVATAR_BORDER_WIDTH;

    /* Render avatar border */
    lcd_fill_rect(left, top,
                  LCD_UI_AVATAR_WIDTH + LCD_UI_AVATAR_BORDER_WIDTH * 2,
                  LCD_UI_AVATAR_HEIGHT + LCD_UI_AVATAR_BORDER_WIDTH * 2,
                  LCD_UI_COLOR_BORDER);

    /* Render avatar */
    lcd_fill_rect(left + LCD_UI_AVATAR_BORDER_WIDTH,
                  top + LCD_UI_AVATAR_BORDER_WIDTH, LCD_UI_AVATAR_WIDTH,
                  LCD_UI_AVATAR_HEIGHT, get_player_avatar(player));
  }
}

static void render_lcd_square(uint8_t x, uint8_t y, lcd_color color) {
  lcd_fill_rect(x * LCD_SQUARE_SIZE,
                y * LCD_SQUARE_SIZE,
                LCD_SQUARE_SIZE,
                LCD_SQUARE_SIZE,
                color);
}

void render_player_to_lcd(player_t *player) {
  uint8_t id = player->player_id - 1;
  uint8_t top = LCD_UI_BORDER_WIDTH + (120 - LCD_UI_BORDER_WIDTH / 2) * id;
  uint8_t left = 240 + LCD_UI_BORDER_WIDTH;

  if (get_player_flag(player, PLAYER_FLAG_SCORE_UPDATED)) {
    lcd_render_integer(left + LCD_UI_SCORE_OFFSET_X,
                       top + LCD_UI_SCORE_OFFSET_Y, player->score,
                       LCD_UI_COLOR_SCORE, LCD_UI_COLOR_BACKGROUND, 2,
                       80 - LCD_UI_BORDER_WIDTH * 2 - LCD_UI_SCORE_OFFSET_X);
  }

  if (get_player_flag(player, PLAYER_FLAG_HEALTH_UPDATED)) {
    uint16_t bar_width = 80 - LCD_UI_BORDER_WIDTH * 2;
    uint16_t health_pixels = player->lives * bar_width / PLAYER_DEFAULT_LIVES;
    uint16_t damage_pixels = bar_width - health_pixels;

    lcd_fill_rect(left, top + LCD_UI_HEALTH_BAR_OFFSET_Y +
                            LCD_UI_HEALTH_BAR_BORDER_WIDTH,
                  health_pixels,
                  LCD_UI_HEALTH_BAR_HEIGHT - LCD_UI_HEALTH_BAR_BORDER_WIDTH * 2,
                  LCD_UI_HEALTH_BAR_COLOR_1);
    lcd_fill_rect(left + health_pixels, top + LCD_UI_HEALTH_BAR_OFFSET_Y +
                                            LCD_UI_HEALTH_BAR_BORDER_WIDTH,
                  damage_pixels,
                  LCD_UI_HEALTH_BAR_HEIGHT - LCD_UI_HEALTH_BAR_BORDER_WIDTH * 2,
                  LCD_UI_HEALTH_BAR_COLOR_2);
  }
}

void render_to_lcd(uint8_t x, uint8_t y) {
  uint8_t i;
  player_t *player;
  for (i = 0; i < PLAYER_COUNT; i++) {
    player = get_player(i + 1);
    if (player->x == x && player->y == y && player->lives) {
      render_lcd_square(x, y, get_player_avatar(player));
      return;
    }
  }

  tile_t *tile = world_tile(x, y);
  switch (tile_type(*tile)) {
  case TILE_TYPE_EMPTY:
    if (tile_contains_bomb(*tile)) {
      render_lcd_square(x, y, LCD_COLOR_BOMB);
    } else if (tile_contains_explosion(*tile)) {
      render_lcd_square(x, y, LCD_COLOR_EXPLOSION);
    } else {
      tile_upgrade_t upgrade = tile_upgrade(*tile);
      switch (upgrade) {
      case TILE_UPGRADE_SPEED:
        render_lcd_square(x, y, LCD_COLOR_UPGRADE_SPEED);
        lcd_render_character(x * LCD_SQUARE_SIZE + 4,
                             y * LCD_SQUARE_SIZE,
                             FONT_CHAR_s,
                             RGB(0, 0, 0),
                             LCD_COLOR_UPGRADE_SPEED,
                             2);
        break;
      case TILE_UPGRADE_BOMBS:
        render_lcd_square(x, y, LCD_COLOR_UPGRADE_BOMBS);
        lcd_render_character(x * LCD_SQUARE_SIZE + 4,
                             y * LCD_SQUARE_SIZE,
                             FONT_CHAR_b,
                             RGB(0, 0, 0),
                             LCD_COLOR_UPGRADE_BOMBS,
                             2);

        break;
      case TILE_UPGRADE_RANGE:
        render_lcd_square(x, y, LCD_COLOR_UPGRADE_RANGE);
        lcd_render_character(x * LCD_SQUARE_SIZE + 4,
                             y * LCD_SQUARE_SIZE,
                             FONT_CHAR_r,
                             RGB(0, 0, 0),
                             LCD_COLOR_UPGRADE_RANGE,
                             2);
        break;
      case TILE_UPGRADE_NONE: /* no options left */
        render_lcd_square(x, y, LCD_COLOR_UPGRADE_NONE);
        break;
      }
    }
    break;

  case TILE_TYPE_SOLID:
    render_lcd_square(x, y, LCD_COLOR_SOLID);
    break;

  case TILE_TYPE_STATIC:
    render_lcd_square(x, y, LCD_COLOR_STATIC);
    break;

  default:
    render_lcd_square(x, y, LCD_COLOR_UNKNOWN);
    break;
  }
}

void update_lcd_brightness() {
  lcd_set_brightness(brightness_control_brightness());
}
