#include <stdint.h>
#include <avr/pgmspace.h>
#include <pleasant-lcd.h>
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

#ifdef INCLUDE_CHARACTERS
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
  C   765432 header 
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

uint8_t render_data(int offsetX,
                    int offsetY,
                    int location,
                    lcd_color textColor,
                    lcd_color bgColor,
                    uint8_t scale) {
  /* Get start of character */
  const PROGMEM uint8_t *ptr;
  ptr = (uint8_t *)&font_PGM[location];

  /* Allocate variables */
  uint8_t byte, j, w, h, width, verticalCharacterOffset, bit, bitPosition,
      height, header;
  uint16_t i;

  /* Read header */
  header = pgm_read_byte(ptr);
  height = (header & 0x03) + 5;
  width = ((header >> 2) & 0x07) + 1;
  verticalCharacterOffset = (header >> 5) & 0x03;
  bit = (header >> 7) & 1;
  bitPosition = 7;

  /* Set area for lcd */
  lcd_batch_start(offsetX, offsetY, width * scale,
                  (height + verticalCharacterOffset) * scale);

  /* Print vertical offset empty area */
  i = 0;
  while (i++ < width * verticalCharacterOffset * scale * scale) {
    lcd_batch_draw(bgColor);
  }

  for (h = 0; h < height; h++) {
    uint8_t _bit, _bitPosition, _byte;
    const PROGMEM uint8_t *_ptr;

    /* Print row 'scale' times */
    for (j = 0; j < scale; j++) {
      /* Set all needed vars to temps */
      _bit = bit;
      _bitPosition = bitPosition;
      _byte = byte;
      _ptr = ptr;

      /* Print row using the temp vars */
      for (w = 0; w < width; w++) {
        /* Draw bit */
        for (i = 0; i < scale; i++) {
          lcd_batch_draw(_bit ? textColor : bgColor);
        }

        /* Goto next bit */
        _bitPosition++;
        if (_bitPosition == 8) {
          _bitPosition = 0;
          _byte = pgm_read_byte(++_ptr);
        }
        _bit = (_byte >> _bitPosition) & 1;
      }
    }

    /* Row is printed 'scale' times, so save temp vars */
    bit = _bit;
    bitPosition = _bitPosition;
    byte = _byte;
    ptr = _ptr;
  }

  /* Stop drawing */
  lcd_batch_stop();

  /* Return the printed width */
  return width * scale;
}

static uint8_t render_digit(int offsetX,
                            int offsetY,
                            uint8_t c,
                            lcd_color textColor,
                            lcd_color bgColor,
                            uint8_t scale) {
  return render_data(offsetX, offsetY, c * 3, textColor, bgColor, scale);
}

void lcd_render_integer(uint16_t offsetX,
                        uint16_t offsetY,
                        int number,
                        lcd_color textColor,
                        lcd_color bgColor,
                        uint8_t scale,
                        uint16_t minWidth) {
  uint16_t lastExponent, d, e, renderedWidth;
  renderedWidth = 0;
  lastExponent = 0;

  /* Print significant digits */
  do {
    d = number;
    e = 1;
    lastExponent = 0;
    while (d > 9) {
      d /= 10;
      e *= 10;
      lastExponent++;
    }
    renderedWidth += render_digit(offsetX + renderedWidth, offsetY, d,
                                  textColor, bgColor, scale);

    lcd_fill_rect(offsetX + renderedWidth, offsetY, scale, scale * 7, bgColor);
    renderedWidth += scale;
    number -= e * d;
  } while (number);

  /* Print trailing 0's */
  for (d = 0; d < lastExponent; d++) {
    renderedWidth += render_digit(offsetX + renderedWidth, offsetY, 0,
                                  textColor, bgColor, scale);
    lcd_fill_rect(offsetX + renderedWidth, offsetY, scale, scale * 7, bgColor);
    renderedWidth += scale;
  }

  /* Print empty space on the end */
  if (renderedWidth < minWidth) {
    lcd_fill_rect(offsetX + renderedWidth, offsetY, minWidth - renderedWidth,
                  scale * 7, bgColor);
  }
}
