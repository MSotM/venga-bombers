#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <avr/pgmspace.h>

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
 * - 1 bits <encoding unit size>
 * - 1 bits <encoding type>
 * - 7 bits unused
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
 */

typedef uint16_t texture_header_t;

#define TEXTURE_HEADER_MASK_SIZE          0b0000000000000111
#define TEXTURE_HEADER_OFFSET_SIZE        0
#define TEXTURE_HEADER_MASK_COLOR_COUNT   0b0000000001111000
#define TEXTURE_HEADER_OFFSET_COLOR_COUNT 3
#define TEXTURE_HEADER_COLOR_COUNT_MAX    16
#define TEXTURE_HEADER_MASK_UNIT_SIZE     0b0000000010000000
#define TEXTURE_HEADER_OFFSET_UNIT_SIZE   7
#define TEXTURE_HEADER_MASK_ENCODING      0b0000000100000000
#define TEXTURE_HEADER_OFFSET_ENCODING    8

typedef struct {
  uint8_t width;
  uint8_t height;
} texture_size_t;

extern const texture_size_t texture_sizes[];
#define TEXTURE_SIZE_16x16  0
#define TEXTURE_SIZE_80x120 1

typedef enum {
  TEXTURE_UNIT_SIZE_2_BITS = (0 << TEXTURE_HEADER_OFFSET_UNIT_SIZE),
  TEXTURE_UNIT_SIZE_4_BITS = (1 << TEXTURE_HEADER_OFFSET_UNIT_SIZE)
} texture_unit_size_t;

typedef enum {
  TEXTURE_ENCODING_PLAIN      = (0 << TEXTURE_HEADER_OFFSET_ENCODING),
  TEXTURE_ENCODING_RUN_LENGTH = (1 << TEXTURE_HEADER_OFFSET_ENCODING)
} texture_encoding_t;

texture_size_t texture_size(texture_header_t header);
uint8_t texture_color_count(texture_header_t header);
texture_unit_size_t texture_unit_size(texture_header_t header);
texture_encoding_t texture_encoding(texture_header_t header);

/* Texture declarations ---------------------------------------------------- */

typedef uint16_t texture_t[];

#define TEXTURE_HEADER(size, color_count, unit_size, encoding)          \
  ((((size) << TEXTURE_HEADER_OFFSET_SIZE)                              \
    & TEXTURE_HEADER_MASK_SIZE)                                         \
   | ((color_count << TEXTURE_HEADER_OFFSET_COLOR_COUNT)                \
      & TEXTURE_HEADER_MASK_COLOR_COUNT)                                \
   | unit_size | encoding)

/* Rendering --------------------------------------------------------------- */

void texture_render(const texture_t texture,
                    uint16_t x,
                    uint16_t y,
                    uint8_t scale);

#endif /* TEXTURE_H */
