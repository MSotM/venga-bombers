#include <pleasant-lcd.h>
#include <stdlib.h>
#include "game.h"
#include "texture.h"
#include "touch-buttons.h"

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
}

static void lcd_render_integer(int number,
                               uint16_t x,
                               uint16_t y,
                               lcd_color text_color,
                               lcd_color background_color,
                               uint8_t scale) {
  uint8_t i;
  const texture_t *texture;

  char buf[6];
  itoa(number, buf, 10);

  texture_set_secondary_colors(text_color, background_color);

  for (i = 0; buf[i] != '\0'; i++) {
    switch (buf[i]) {
    case '0': texture = &TEXTURE_CHARACTER_0; break;
    case '1': texture = &TEXTURE_CHARACTER_1; break;
    case '2': texture = &TEXTURE_CHARACTER_2; break;
    case '3': texture = &TEXTURE_CHARACTER_3; break;
    case '4': texture = &TEXTURE_CHARACTER_4; break;
    case '5': texture = &TEXTURE_CHARACTER_5; break;
    case '6': texture = &TEXTURE_CHARACTER_6; break;
    case '7': texture = &TEXTURE_CHARACTER_7; break;
    case '8': texture = &TEXTURE_CHARACTER_8; break;
    default:  texture = &TEXTURE_CHARACTER_9; break;
    }

    texture_render(*texture, x, y, scale);
    x += texture_sizes[TEXTURE_SIZE_CHARACTER].width * scale;
  }
}

static void render_player_status(player_t *player,
                                 uint8_t x_offset,
                                 uint8_t y_offset,
                                 bool force) {
  uint8_t health_bar_filled_width;
  uint8_t health_bar_empty_width;

  if (force) {
    texture_render(TEXTURE_UI, x_offset, y_offset, 1);

    texture_render(*player->texture,
                   x_offset + LCD_UI_AVATAR_OFFSET_X,
                   y_offset + LCD_UI_AVATAR_OFFSET_Y,
                   3);
  }

  if (get_player_flag(player, PLAYER_FLAG_SCORE_UPDATED)) {
    lcd_render_integer(player->score,
                       x_offset + LCD_UI_SCORE_OFFSET_X,
                       y_offset + LCD_UI_SCORE_OFFSET_Y,
                       LCD_UI_COLOR_SCORE,
                       LCD_UI_COLOR_SCORE_BACKGROUND,
                       2);
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
      texture_render(*player->texture,
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

/* Brightness -------------------------------------------------------------- */

void update_lcd_brightness() {
  lcd_set_brightness(brightness_control_brightness());
}

/* Menu -------------------------------------------------------------------- */

void render_menu_background_to_lcd() {
  uint8_t width = LCD_WIDTH / LCD_SQUARE_SIZE;
  uint8_t height = LCD_HEIGHT / LCD_SQUARE_SIZE;
  uint8_t x, y;

  for (x = 0; x < width; x++) {
    for (y = 0; y < height; y++) {
      if (x == 0 || x == width - 1 ||
          y == 0 || y == height - 1) {
        texture_render(TEXTURE_STATIC,
                       x * LCD_SQUARE_SIZE,
                       y * LCD_SQUARE_SIZE,
                       1);
      }

      if (x > 0 && x < width - 1 &&
          y > 0 && y < height - 1) {
        texture_render(TEXTURE_EMPTY,
                       x * LCD_SQUARE_SIZE,
                       y * LCD_SQUARE_SIZE,
                       1);
      }
    }
  }
}

void render_menu_to_lcd() {
  /* Title space */
  uint16_t title_pos_x = 5 * LCD_SQUARE_SIZE;
  uint16_t title_pos_y = 2 * LCD_SQUARE_SIZE;
  texture_render(TEXTURE_MENU_TITLE,
                 title_pos_x,
                 title_pos_y,
                 1);

  texture_render(TEXTURE_PLAY_BUTTON,
                 MENU_BUTTON_PLAY_X,
                 MENU_BUTTON_PLAY_Y,
                 1);

  texture_render(TEXTURE_HIGHSCORES_BUTTON,
                 MENU_BUTTON_HIGHSCORES_X,
                 MENU_BUTTON_HIGHSCORES_Y,
                 1);

  texture_render(TEXTURE_PLAYER_1_DOWN,
                 2 * LCD_SQUARE_SIZE,
                 2 * LCD_SQUARE_SIZE,
                 2);

  texture_render(TEXTURE_PLAYER_2_DOWN,
                 ((LCD_WIDTH / LCD_SQUARE_SIZE) - 4) * LCD_SQUARE_SIZE,
                 2 * LCD_SQUARE_SIZE,
                 2);
}

void render_end_screen_to_lcd(game_result_t result) {
  bool draw_player1 = false;
  bool draw_player2 = false;
  if (result == GAME_RESULT_PLAYER1_WIN) {
    texture_render(TEXTURE_RESULT_PLAYER1_WIN,
                   5 * LCD_SQUARE_SIZE,
                   2 * LCD_SQUARE_SIZE,
                   1);

    draw_player1 = true;
  } else if (result == GAME_RESULT_PLAYER2_WIN) {
    texture_render(TEXTURE_RESULT_PLAYER2_WIN,
                   5 * LCD_SQUARE_SIZE,
                   2 * LCD_SQUARE_SIZE,
                   1);

    draw_player2 = true;
  } else if (result == GAME_RESULT_DRAW){
    texture_render(TEXTURE_RESULT_DRAW,
                   5 * LCD_SQUARE_SIZE,
                   2 * LCD_SQUARE_SIZE,
                   1);

    draw_player1 = true;
    draw_player2 = true;
  }

  if (draw_player1) {
    texture_render(TEXTURE_PLAYER_1_DOWN,
                   2 * LCD_SQUARE_SIZE,
                   2 * LCD_SQUARE_SIZE,
                   2);

    player_t *player1 = get_player(1);
    uint8_t x_offset = (player1->score < 10000) ? 2 : 1;
    lcd_render_integer(player1->score,
                       x_offset * LCD_SQUARE_SIZE,
                       4 * LCD_SQUARE_SIZE,
                       RGB(255, 255, 255),
                       RGB(0, 0, 0), 2);
  }

  if (draw_player2) {
    texture_render(TEXTURE_PLAYER_2_DOWN,
                   ((LCD_WIDTH / LCD_SQUARE_SIZE) - 4) * LCD_SQUARE_SIZE,
                   2 * LCD_SQUARE_SIZE,
                   2);

    player_t *player2 = get_player(2);
    uint8_t world_width = LCD_WIDTH / LCD_SQUARE_SIZE;
    uint8_t x_offset = (player2->score < 10000) ? 4 : 5;
    lcd_render_integer(player2->score,
                       (world_width - x_offset) * LCD_SQUARE_SIZE,
                       4 * LCD_SQUARE_SIZE,
                       RGB(255, 255, 255),
                       RGB(0, 0, 0), 2);
  }

  texture_render(TEXTURE_MENU_BUTTON,
                 END_BUTTON_BACK_TO_MENU_X,
                 END_BUTTON_BACK_TO_MENU_Y,
                 1);

  texture_render(TEXTURE_PLAY_BUTTON,
                 END_BUTTON_PLAY_AGAIN_X,
                 END_BUTTON_PLAY_AGAIN_Y,
                 1);
}

/* Highscores -------------------------------------------------------------- */

#define LCD_HIGHSCORE_COLOR RGB(255, 255, 255)
#define LCD_HIGHSCORE_BACKGROUND_COLOR RGB(0, 0, 0)

#define LCD_HIGHSCORE_X 140
#define LCD_HIGHSCORE_Y 60

void render_highscores_background_to_lcd() {
  render_menu_background_to_lcd();
}

void render_highscores_to_lcd() {
  uint8_t i;
  highscores_t highscores = {0};
  highscores_read(highscores);

  for (i = 0; i < HIGHSCORE_COUNT; i++) {
    lcd_render_integer(highscores[i],
                       LCD_HIGHSCORE_X,
                       LCD_HIGHSCORE_Y + 30 * i,
                       LCD_HIGHSCORE_COLOR,
                       LCD_HIGHSCORE_BACKGROUND_COLOR,
                       2);
  }
}
