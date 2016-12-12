#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <pleasant-lcd.h>

#define FONT_INCLUDES_DIGITS
/* #define FONT_INCLUDES_LETTERS */

#define FONT_CHAR_c 30
#define FONT_CHAR_e 32
#define FONT_CHAR_g 34
#define FONT_CHAR_h 36
#define FONT_CHAR_i 38
#define FONT_CHAR_o 40
#define FONT_CHAR_p 42
#define FONT_CHAR_r 44
#define FONT_CHAR_s 46

void lcd_render_integer(uint16_t offset_x,
                        uint16_t offset_y,
                        int number,
                        lcd_color text_color,
                        lcd_color bg_color,
                        uint8_t scale,
                        uint16_t min_width);

#endif /* FONT_H */
