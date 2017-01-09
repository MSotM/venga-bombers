#ifndef TOUCH_BUTTONS_H
#define TOUCH_BUTTONS_H

/* GENERAL ----------------------------------------------------------------- */

#define NO_BUTTON_PRESSED 255
#define DEFAULT_BUTTON_WIDTH 80
#define DEFAULT_BUTTON_HEIGHT 30

typedef struct {
  uint16_t x, y, width, height;
  uint8_t index;
} touch_button_t;

bool is_button_pressed(uint16_t touch_x,
                       uint16_t touch_y,
                       touch_button_t button);

/* MENU -------------------------------------------------------------------- */

typedef enum {
  MENU_BUTTON_PLAY,
  MENU_BUTTON_HIGHSCORES,
  NUM_MENU_BUTTONS
} touch_menu_button_t;

#define MENU_BUTTON_PLAY_X 120
#define MENU_BUTTON_PLAY_Y 105

#define MENU_BUTTON_HIGHSCORES_X 120
#define MENU_BUTTON_HIGHSCORES_Y 136

/* END --------------------------------------------------------------------- */

typedef enum {
  END_BUTTON_BACK_TO_MENU,
  END_BUTTON_PLAY_AGAIN,
  NUM_END_BUTTONS
} touch_end_button_t;

#define END_BUTTON_BACK_TO_MENU_X 120
#define END_BUTTON_BACK_TO_MENU_Y 145

#define END_BUTTON_PLAY_AGAIN_X 120
#define END_BUTTON_PLAY_AGAIN_Y 105

#endif /* TOUCH_BUTTONS_H */
