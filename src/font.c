#include <avr/pgmspace.h>
#include "font.h"

/*
 * Byte 1:
 *   Bit 0-1: Height of character (5-8)
 *   Bit 2-4: Width of character (1-8)
 *   Bit 5-6: Character offset on vertical axis (0-3)
 *   Bit 7: First bit
 *
 * Following bytes:
 *   number of bytes = Math.ceil(height*width/8)
 *   bytes contain the pixel of the characters in strips along the y axis
 *   A bit of 1 means the pixel is filled, a bit of 0 means the pixel is empty
 */

const PROGMEM uint16_t font_PGM[] = {
#ifdef FONT_INCLUDES_DIGITS
  /* The number 0 (0x30) encoded as follows:
   * .111.
   * 1...1
   * 1..11
   * 1.1.1
   * 11..1
   * 1...1
   * .111.
  R 22221111  header    5555544444333332          7777766666
  C 43215432  header    5432154321543215          5432154321 */
  0b0001011100010010, 0b1001110101110011, 0b0000000111010001,

  /* The number 1 (0x31) encoded as follows:
   * 11
   * .1
   * .1
   * .1
   * .1
   * .1
   * .1
  R 54433221 header                77665
  C 12121212 header                21212 */
  0b0101010110000110, 0b0000000000010101, 0x00,

  /* The number 2 (0x32) encoded as follows:
   * 1111.
   * ....1
   * ....1
   * .111.
   * 1....
   * 1....
   * 11111
  R 22221111  header    5555544444333332          7777766666
  C 43215432  header    5432154321543215          5432154321 */
  0b0000011110010010, 0b0000101110100001, 0b0000001111100001,

  /* The number 3 (0x33) encoded as follows:
   * 111.
   * ...1
   * ...1
   * 111.
   * ...1
   * ...1
   * 111.
  R 32222111 header     7666655554444333                 777
  C 14321432 header     1432143214321432                 432 */
  0b0100001110001110, 0b1100010000111100, 0b0000000000000011,

  /* The number 4 (0x34) encoded as follows:
   * ...11
   * ..1.1
   * .1..1
   * 1...1
   * 11111
   * ....1
   * ....1
  R 22221111  header    5555544444333332          7777766666
  C 43215432  header    5432154321543215          5432154321 */
  0b0100110000010010, 0b1111110001100101, 0b0000001000010000,

  /* The number 5 (0x35) encoded as follows:
   * 11111
   * 1....
   * 1....
   * 1111.
   * ....1
   * ....1
   * 1111.
  R 22221111  header    5555544444333332          7777766666
  C 43215432  header    5432154321543215          5432154321 */
  0b0001111110010010, 0b1000001111000010, 0b0000000111110000,

  /* The number 6 (0x36) encoded as follows:
   * .111.
   * 1....
   * 1....
   * 1111.
   * 1...1
   * 1...1
   * .111.
  R 22221111  header    5555544444333332          7777766666
  C 43215432  header    5432154321543215          5432154321 */
  0b0001011100010010, 0b1000101111000010, 0b0000000111010001,

  /* The number 7 (0x37) encoded as follows:
   * 11111
   * ....1
   * ...1.
   * ...1.
   * ..1..
   * ..1..
   * ..1..
  R 22221111  header    5555544444333332          7777766666
  C 43215432  header    5432154321543215          5432154321 */
  0b0000111110010010, 0b0010001000010001, 0b0000000010000100,

  /* The number 8 (0x38) encoded as follows:
   * .111.
   * 1...1
   * 1...1
   * .111.
   * 1...1
   * 1...1
   * .111.
  R 22221111  header    5555544444333332          7777766666
  C 43215432  header    5432154321543215          5432154321 */
  0b0001011100010010, 0b1000101110100011, 0b0000000111010001,

  /* The number 9 (0x39) encoded as follows:
   * .111.
   * 1...1
   * 1...1
   * .1111
   * ....1
   * ....1
   * .111.
  R 22221111  header    5555544444333332          7777766666
  C 43215432  header    5432154321543215          5432154321 */
  0b0001011100010010, 0b1000011110100011, 0b0000000111010000,
#endif
#ifdef FONT_INCLUDES_LETTERS
  /* The character b encoded as follows:
   * 1...
   * 1...
   * 111.
   * 1..1
   * 1..1
   * 111.
  R 22221111 header     5555544444333332
  C 43215432 header     5432154321543215 */
  0b1000100010101101, 0b0011110011001011,

  /* The character c encoded as follows:
   * .111.
   * 1...1
   * 1....
   * 1...1
   * .111.
  R 22221111 header     5555544444333332
  C 43215432 header     5432154321543215 */
  0b0001011101010000, 0b0111010001000011,

  /* The character e encoded as follows:
   * .111.
   * 1...1
   * 11111
   * 1....
   * .111.
  R 22221111 header     5555544444333332
  C 43215432 header     5432154321543215 */
  0b0001011101010000, 0b0111000001111111,

  /* The character g encoded as follows:
   * .11.
   * 1..1
   * 1..1
   * .111
   * ...1
   * .11.
  R 32222111 header      666655554444333
  C 14321432 header      432143214321432 */
  0b1100101101101101, 0b0011010001110100,

  /* The character h encoded as follows:
   * 1...
   * 1...
   * 111.
   * 1..1
   * 1..1
   * 1..1
  R 32222111 header      666655554444333
  C 14321432 header      432143214321432 */
  0b1000100010101101, 0b0100110011001011,

  /* The character i encoded as follows:
   * 1
   * .
   * 1
   * 1
   * 1
   * 1
   * 1
  R   111111 header
  C   765432 header */
  0b0011111010000010, 0b0000000000000000,

  /* The character o encoded as follows:
   * .111.
   * 1...1
   * 1...1
   * 1...1
   * .111.
  R 22221111 header     5555544444333332
  C 43215432 header     5432154321543215 */
  0b0001011101010000, 0b0111010001100011,

  /* The character p encoded as follows:
   * .111.
   * 1...1
   * 1...1
   * 1...1
   * .111.
  R 22221111 header     5555544444333332
  C 43215432 header     5432154321543215 */
  0b0001011101010000, 0b0111010001100011,

  /* The character r encoded as follows:
   * 1.11
   * 11..
   * 1...
   * 1...
   * 1...
  R 32222111 header          55554444333
  C 14321432 header          43214321432 */
  0b1001111011001100, 0b0000000010001000,

  /* The character s encoded as follows:
   * .111
   * 1...
   * .11.
   * ...1
   * 111.
  R 32222111 header          55554444333
  C 14321432 header          43214321432 */
  0b0000111101001100, 0b0000001111000011,
#endif
};

typedef struct {
  uint8_t *byte_pointer;
  uint8_t byte;
  uint8_t bit_position;
  uint8_t width, height;
  uint8_t vertical_offset;
} character_render_state_t;

static void parse_character_header(character_render_state_t *state) {
  uint8_t byte = state->byte;

  state->width = ((byte >> 2) & 0x07) + 1;
  state->height = (byte & 0x03) + 5;
  state->vertical_offset = (byte >> 5) & 0x03;
  state->bit_position = 7;
}

static void render_row(character_render_state_t *state,
                       uint8_t scale,
                       lcd_color text_color,
                       lcd_color bg_color) {
  uint8_t i, w, j;

  /* Each row of the character needs to be rendered multiple times, to account
     for the scale. Each iteration should render the exact same thing, so we
     need to copy the appropriate state and only update it after all iterations
     are done. */
  uint8_t *byte_pointer, byte, bit_position, bit;

  if (scale < 1) return;

  for (i = 0; i < scale; i++) {
    byte_pointer = state->byte_pointer;
    byte = state->byte;
    bit_position = state->bit_position;
    bit = (byte >> bit_position) & 1;

    for (w = 0; w < state->width; w++) {
      /* Width of each pixel also needs to be scaled */
      for (j = 0; j < scale; j++) {
        lcd_batch_draw(bit ? text_color : bg_color);
      }

      bit_position++;
      if (bit_position > 7) {
        bit_position = 0;
        byte = pgm_read_byte(++byte_pointer);
      }
      bit = (byte >> bit_position) & 1;
    }
  }

  state->byte_pointer = byte_pointer;
  state->byte = byte;
  state->bit_position = bit_position;
}

uint8_t lcd_render_character(int offset_x,
                             int offset_y,
                             int location,
                             lcd_color text_color,
                             lcd_color bg_color,
                             uint8_t scale) {
  character_render_state_t state;
  uint16_t i;

  state.byte_pointer = (uint8_t *)&font_PGM[location];
  state.byte = pgm_read_byte(state.byte_pointer);
  parse_character_header(&state);

  lcd_batch_start(offset_x,
                  offset_y,
                  state.width * scale,
                  (state.height + state.vertical_offset) * scale);

  /* We need to fill the area taken up by the vertical offset with the
     background color, to make sure it's clear. */
  for (i = 0;
       i < ((state.width * scale) * (state.vertical_offset * scale));
       i++) {
    lcd_batch_draw(bg_color);
  }

  for (i = 0; i < state.height; i++) {
    render_row(&state, scale, text_color, bg_color);
  }

  lcd_batch_stop();
  return state.width * scale;
}

static uint8_t render_digit(int offset_x,
                            int offset_y,
                            uint8_t c,
                            lcd_color text_color,
                            lcd_color bg_color,
                            uint8_t scale) {
#ifndef FONT_INCLUDES_DIGITS
  return 0;
#endif
  return lcd_render_character(offset_x,
                              offset_y,
                              c * 3,
                              text_color,
                              bg_color,
                              scale);
}

void lcd_render_integer(uint16_t offset_x,
                        uint16_t offset_y,
                        int number,
                        lcd_color text_color,
                        lcd_color bg_color,
                        uint8_t scale,
                        uint16_t min_width) {
  uint16_t last_exponent, d, e, rendered_width;
  rendered_width = 0;
  last_exponent = 0;

  /* Print significant digits */
  do {
    d = number;
    e = 1;
    last_exponent = 0;
    while (d > 9) {
      d /= 10;
      e *= 10;
      last_exponent++;
    }
    rendered_width += render_digit(offset_x + rendered_width,
                                   offset_y,
                                   d,
                                   text_color,
                                   bg_color,
                                   scale);

    lcd_fill_rect(offset_x + rendered_width,
                  offset_y,
                  scale,
                  scale * 7,
                  bg_color);
    rendered_width += scale;
    number -= e * d;
  } while (number);

  /* Print trailing 0's */
  for (d = 0; d < last_exponent; d++) {
    rendered_width += render_digit(offset_x + rendered_width,
                                  offset_y,
                                  0,
                                  text_color,
                                  bg_color,
                                  scale);
    lcd_fill_rect(offset_x + rendered_width,
                  offset_y,
                  scale,
                  scale * 7,
                  bg_color);
    rendered_width += scale;
  }

  /* Print empty space on the end */
  if (rendered_width < min_width) {
    lcd_fill_rect(offset_x + rendered_width,
                  offset_y,
                  min_width - rendered_width,
                  scale * 7,
                  bg_color);
  }
}
