#include <avr/pgmspace.h>
#include "texture.h"

const texture_size_t texture_sizes[] = {
  {16, 16},
  {80, 120},
  {5,  8},
  {80, 30},
  {160, 32},
};

/* Texture accessors ------------------------------------------------------- */

texture_size_t texture_size(texture_header_t header) {
  return texture_sizes[((header & TEXTURE_HEADER_MASK_SIZE)
                        >> TEXTURE_HEADER_OFFSET_SIZE)];
}

uint8_t texture_color_count(texture_header_t header) {
  return ((header & TEXTURE_HEADER_MASK_COLOR_COUNT)
          >> TEXTURE_HEADER_OFFSET_COLOR_COUNT);
}

texture_unit_size_t texture_unit_size(texture_header_t header) {
  return header & TEXTURE_HEADER_MASK_UNIT_SIZE;
}

texture_encoding_t texture_encoding(texture_header_t header) {
  return header & TEXTURE_HEADER_MASK_ENCODING;
}

/* Render state ------------------------------------------------------------ */

const lcd_color TEXTURE_SECONDARY_COLOR_INDICATOR_1 = RGB(251, 251, 251);
const lcd_color TEXTURE_SECONDARY_COLOR_INDICATOR_2 = RGB(247, 247, 247);

static lcd_color secondary_colors[] = {
  RGB(0, 0, 0),
  RGB(255, 255, 255),
};

void texture_set_secondary_colors(lcd_color secondary_color_1,
                                  lcd_color secondary_color_2) {
  secondary_colors[0] = secondary_color_1;
  secondary_colors[1] = secondary_color_2;
}

static struct {
  texture_size_t size;
  uint8_t color_count;
  texture_encoding_t encoding;
  /* Calculated properties */
  uint8_t unit_size;
  uint16_t unit_mask;
  lcd_color colors[TEXTURE_HEADER_COLOR_COUNT_MAX];
} current_texture;

typedef struct {
  /* These change all the time during a render pass, and need to be saved when
     rendering rows multiple times to account for scale. */
  const uint16_t *word_pointer;
  uint16_t word;
  uint8_t unit_offset;
  uint8_t unit;
  uint8_t run_length;
} render_state_t;

static render_state_t render_state;
static render_state_t saved_render_state;

static void save_render_state() { saved_render_state = render_state; }
static void restore_render_state() { render_state = saved_render_state; }

/* Parsing ----------------------------------------------------------------- */

static uint8_t next_unit_value() {
  if (render_state.unit_offset <= 0) {
    render_state.word = pgm_read_word(++render_state.word_pointer);
    render_state.unit_offset = 16;
  }

  render_state.unit_offset -= current_texture.unit_size;
  render_state.unit = ((render_state.word >> render_state.unit_offset)
                       & current_texture.unit_mask);

  return render_state.unit;
}

static lcd_color next_pixel_value() {
  switch (current_texture.encoding) {
  case TEXTURE_ENCODING_PLAIN:
    /* Set up render_state.unit to contain the color index */
    next_unit_value();
    break;
  case TEXTURE_ENCODING_RUN_LENGTH:
    if (render_state.run_length-- <= 1) {
      render_state.run_length = next_unit_value();
      /* Set up render_state.unit to contain the color index */
      next_unit_value();
    }
    break;
  }

  return current_texture.colors[render_state.unit];
}

static uint8_t texture_integer_unit_size(texture_header_t header) {
  switch (texture_unit_size(header)) {
  case TEXTURE_UNIT_SIZE_1_BITS: return 1;
  case TEXTURE_UNIT_SIZE_2_BITS: return 2;
  case TEXTURE_UNIT_SIZE_4_BITS: return 4;
  case TEXTURE_UNIT_SIZE_8_BITS: return 8;
  default:                       return 0;
  }
}

static uint8_t texture_unit_mask(uint8_t integer_unit_size) {
  switch (integer_unit_size) {
  case 1: return 0b1;
  case 2: return 0b11;
  case 4: return 0b1111;
  case 8: return 0b11111111;
  default: return 0;
  }
}

static void parse_texture(const texture_t texture) {
  uint8_t i;
  lcd_color color;

  render_state.word_pointer = texture;
  render_state.word = pgm_read_word(render_state.word_pointer);

  current_texture.size = texture_size(render_state.word);
  current_texture.color_count = texture_color_count(render_state.word);
  current_texture.encoding = texture_encoding(render_state.word);

  render_state.unit_offset = 0;

  current_texture.unit_size = texture_integer_unit_size(render_state.word);
  current_texture.unit_mask = texture_unit_mask(current_texture.unit_size);

  for (i = 0; i < current_texture.color_count; i++) {
    color = pgm_read_word(++render_state.word_pointer);

    if (color == (lcd_color)TEXTURE_SECONDARY_COLOR_INDICATOR_1) {
      color = secondary_colors[0];
    } else if (color == (lcd_color)TEXTURE_SECONDARY_COLOR_INDICATOR_2) {
      color = secondary_colors[1];
    }

    current_texture.colors[i] = color;
  }
}

/* Rendering --------------------------------------------------------------- */

static void render_current_texture_scaled(uint8_t scale) {
  uint8_t x, y, i, j;
  lcd_color pixel;

  /* We need to loop through each row in the texture <scale> times, and then we
     need to loop through each pixel in that row <scale> times as well. */
  for (y = 0; y < current_texture.size.height; y++) {
    save_render_state();

    for (i = 0; i < scale; i++) {
      restore_render_state();
      for (x = 0; x < current_texture.size.width; x++) {
        pixel = next_pixel_value();
        for (j = 0; j < scale; j++) {
          lcd_batch_draw(pixel);
        }
      }
    }
  }
}

void texture_render(const texture_t texture,
                    uint16_t x,
                    uint16_t y,
                    uint8_t scale) {
  if (scale < 1) return;

  parse_texture(texture);

  lcd_batch_start(x,
                  y,
                  current_texture.size.width * scale,
                  current_texture.size.height * scale);

  render_current_texture_scaled(scale);

  lcd_batch_stop();
}
