#include <pleasant-lcd.h>
#include <stdio.h>
#include "game.h"

#define LCD_SQUARE_SIZE 16

#define LCD_COLOR_PLAYER1       RGB(0,   255, 0  )
#define LCD_COLOR_PLAYER2       RGB(0,   0,   255)
#define LCD_COLOR_BOMB          RGB(255, 0,   0  )
#define LCD_COLOR_EXPLOSION     RGB(255, 128, 0  )
#define LCD_COLOR_SOLID         RGB(128, 128, 128)
#define LCD_COLOR_STATIC        RGB(0,   0,   0  )
#define LCD_COLOR_UPGRADE_SPEED RGB(0,   255, 255)
#define LCD_COLOR_UPGRADE_BOMBS RGB(0,   255, 255)
#define LCD_COLOR_UPGRADE_RANGE RGB(0,   255, 255)
#define LCD_COLOR_UPGRADE_NONE  RGB(255, 255, 255)
#define LCD_COLOR_UNKNOWN       RGB(255, 0,   255)

void init_lcd_display() {
  lcd_init(LCD_DEFAULT_SPI_CLOCK_SPEED);
  lcd_set_brightness(100);
  lcd_fill_screen(RGB(255, 255, 255));
}

static void render_lcd_square(uint8_t x, uint8_t y, lcd_color color) {
  lcd_fill_rect(x * LCD_SQUARE_SIZE,
                y * LCD_SQUARE_SIZE,
                LCD_SQUARE_SIZE,
                LCD_SQUARE_SIZE,
                color);
}

void render_to_lcd(uint8_t x, uint8_t y) {
  if (get_player(1)->x == x &&
      get_player(1)->y == y &&
      get_player(1)->lives) {
    render_lcd_square(x, y, LCD_COLOR_PLAYER1);
  } else if (get_player(2)->x == x &&
             get_player(2)->y == y &&
             get_player(2)->lives) {
    render_lcd_square(x, y, LCD_COLOR_PLAYER2);
  } else {
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
              break;
            case TILE_UPGRADE_BOMBS:
              render_lcd_square(x, y, LCD_COLOR_UPGRADE_BOMBS);
              break;
            case TILE_UPGRADE_RANGE:
              render_lcd_square(x, y, LCD_COLOR_UPGRADE_RANGE);
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
}
