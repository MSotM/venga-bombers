#ifndef TOUCH_BUTTONS_H
#define TOUCH_BUTTONS_H

#define NO_BUTTON_PRESSED 255

typedef enum {
  MENU_BUTTON_PLAY,
  NUM_MENU_BUTTONS
} touch_menu_button_t;

typedef struct {
  uint16_t x, y, width, height;
  touch_menu_button_t index;
} touch_button_t;

bool is_button_pressed(uint16_t touch_x,
                       uint16_t touch_y,
                       touch_button_t button) {
  if (touch_x >= button.x && touch_x <= button.x + button.width &&
      touch_y >= button.y && touch_y <= button.y + button.height) {
    return true;
  }

  return false;
}

#endif /* TOUCH_BUTTONS_H */
