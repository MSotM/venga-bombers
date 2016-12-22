#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include <pleasant-lcd.h>

/* Texture ----------------------------------------------------------------- */

/*
 * +---------------+---------------------+--------------------+
 * | 1 word header | n words color table | m words pixel data |
 * +---------------+---------------------+--------------------+
 *
 * The first word of an image is its header. It contains the following data:
 *
 * - 3 bits <texture size>
 * - 4 bits <number of colors>
 * - 2 bits <encoding unit size>
 * - 1 bits <encoding type>
 * - 6 bits unused
 *
 * From these bits, we can calculate how many bits are needed for indexes into
 * the color table.
 *
 * There are two types of encoding, plain and run-length. Both exist in two
 * variants, with a unit size of 2 or 4 bits.
 *
 * When run-length encoding is not used, the pixel data is encoded as a
 * sequence of indexes into the color table, one for each pixel in the texture.
 *
 * When run-length encoding is used, pixel data is encoded as pairs of a
 * run-length and a color table index, where the run-length specifies how many
 * pixels should be filled with the associated color.
 *
 * In both cases any bits left in the final word are unused.
 *
 * The color table immediately follows the header, and consists of <number of
 * colors> words. There are two special colors which, when encountered, will
 * not actually be used in rendering the image. They will instead be read from
 * a secondary color table. These are TEXTURE_SECONDARY_COLOR_INDICATOR_1 and
 * TEXTURE_SECONDARY_COLOR_INDICATOR_2.
 */

typedef uint16_t texture_header_t;

#define TEXTURE_HEADER_MASK_SIZE          0b0000000000000111
#define TEXTURE_HEADER_OFFSET_SIZE        0
#define TEXTURE_HEADER_MASK_COLOR_COUNT   0b0000000001111000
#define TEXTURE_HEADER_OFFSET_COLOR_COUNT 3
#define TEXTURE_HEADER_COLOR_COUNT_MAX    16
#define TEXTURE_HEADER_MASK_UNIT_SIZE     0b0000000110000000
#define TEXTURE_HEADER_OFFSET_UNIT_SIZE   7
#define TEXTURE_HEADER_MASK_ENCODING      0b0000001000000000
#define TEXTURE_HEADER_OFFSET_ENCODING    9

typedef struct {
  uint8_t width;
  uint8_t height;
} texture_size_t;

extern const texture_size_t texture_sizes[];

#define TEXTURE_SIZE_TILE      0
#define TEXTURE_SIZE_UI        1
#define TEXTURE_SIZE_CHARACTER 2

typedef enum {
  TEXTURE_UNIT_SIZE_1_BITS = (0 << TEXTURE_HEADER_OFFSET_UNIT_SIZE),
  TEXTURE_UNIT_SIZE_2_BITS = (1 << TEXTURE_HEADER_OFFSET_UNIT_SIZE),
  TEXTURE_UNIT_SIZE_4_BITS = (2 << TEXTURE_HEADER_OFFSET_UNIT_SIZE),
  TEXTURE_UNIT_SIZE_8_BITS = (3 << TEXTURE_HEADER_OFFSET_UNIT_SIZE)
} texture_unit_size_t;

typedef enum {
  TEXTURE_ENCODING_PLAIN      = (0 << TEXTURE_HEADER_OFFSET_ENCODING),
  TEXTURE_ENCODING_RUN_LENGTH = (1 << TEXTURE_HEADER_OFFSET_ENCODING)
} texture_encoding_t;

texture_size_t texture_size(texture_header_t header);
uint8_t texture_color_count(texture_header_t header);
texture_unit_size_t texture_unit_size(texture_header_t header);
texture_encoding_t texture_encoding(texture_header_t header);

extern const lcd_color TEXTURE_SECONDARY_COLOR_INDICATOR_1;
extern const lcd_color TEXTURE_SECONDARY_COLOR_INDICATOR_2;

void texture_set_secondary_colors(lcd_color secondary_color_1,
                                  lcd_color secondary_color_2);

/* Texture declarations -------------------------------------------------------
 * Definitions of these textures are in textures.c
 */

typedef uint16_t texture_t[];

#define TEXTURE_HEADER(size, color_count, unit_size, encoding)          \
  ((((size) << TEXTURE_HEADER_OFFSET_SIZE)                              \
    & TEXTURE_HEADER_MASK_SIZE)                                         \
   | ((color_count << TEXTURE_HEADER_OFFSET_COLOR_COUNT)                \
      & TEXTURE_HEADER_MASK_COLOR_COUNT)                                \
   | unit_size | encoding)

extern const texture_t TEXTURE_STATIC;
extern const texture_t TEXTURE_SOLID;
extern const texture_t TEXTURE_EMPTY;
extern const texture_t TEXTURE_EXPLOSION;
extern const texture_t TEXTURE_BOMB_PLAYER_1;
extern const texture_t TEXTURE_BOMB_PLAYER_2;
extern const texture_t TEXTURE_PLAYER_1_DOWN;
extern const texture_t TEXTURE_PLAYER_1_UP;
extern const texture_t TEXTURE_PLAYER_1_RIGHT;
extern const texture_t TEXTURE_PLAYER_1_LEFT;
extern const texture_t TEXTURE_PLAYER_2_DOWN;
extern const texture_t TEXTURE_PLAYER_2_UP;
extern const texture_t TEXTURE_PLAYER_2_RIGHT;
extern const texture_t TEXTURE_PLAYER_2_LEFT;
extern const texture_t TEXTURE_UPGRADE_RANGE;
extern const texture_t TEXTURE_UPGRADE_BOMBS;
extern const texture_t TEXTURE_UPGRADE_SPEED;
extern const texture_t TEXTURE_ERROR;
extern const texture_t TEXTURE_UI;

extern const texture_t TEXTURE_CHARACTER_0;
extern const texture_t TEXTURE_CHARACTER_1;
extern const texture_t TEXTURE_CHARACTER_2;
extern const texture_t TEXTURE_CHARACTER_3;
extern const texture_t TEXTURE_CHARACTER_4;
extern const texture_t TEXTURE_CHARACTER_5;
extern const texture_t TEXTURE_CHARACTER_6;
extern const texture_t TEXTURE_CHARACTER_7;
extern const texture_t TEXTURE_CHARACTER_8;
extern const texture_t TEXTURE_CHARACTER_9;

/* Rendering --------------------------------------------------------------- */

void texture_render(const texture_t texture,
                    uint16_t x,
                    uint16_t y,
                    uint8_t scale);

#endif /* TEXTURE_H */
