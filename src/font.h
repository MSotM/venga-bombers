#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <pleasant-lcd.h>

#define FONT_INCLUDES_DIGITS
/* #define FONT_INCLUDES_LETTERS */

#define FONT_CHAR_b 30
#define FONT_CHAR_c 32
#define FONT_CHAR_e 34
#define FONT_CHAR_g 36
#define FONT_CHAR_h 38
#define FONT_CHAR_i 40
#define FONT_CHAR_o 42
#define FONT_CHAR_p 44
#define FONT_CHAR_r 46
#define FONT_CHAR_s 48

void lcd_render_integer(uint16_t offset_x,
                        uint16_t offset_y,
                        int number,
                        lcd_color text_color,
                        lcd_color bg_color,
                        uint8_t scale,
                        uint16_t min_width);

#endif /* FONT_H */
