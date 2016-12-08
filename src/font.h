#ifndef GAME_H
#define GAME_H

#define FONT_CHAR_c 30
#define FONT_CHAR_e 32
#define FONT_CHAR_g 34
#define FONT_CHAR_h 36
#define FONT_CHAR_i 38
#define FONT_CHAR_o 40
#define FONT_CHAR_p 42
#define FONT_CHAR_r 44
#define FONT_CHAR_s 46

void lcd_render_integer(uint16_t offsetX, uint16_t offsetY, int number,
                        lcd_color textColor, lcd_color bgColor, uint8_t scale,
                        uint16_t minWidth);

uint8_t render_data(int offsetX, int offsetY, int location, lcd_color textColor,
                    lcd_color bgColor, uint8_t scale);

#endif
