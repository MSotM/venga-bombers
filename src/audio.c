#include <stdbool.h>
#include <avr/io.h>
#include "audio.h"
#include "timeout.h"

/* Distinct rhythm durations */
static const uint16_t rhythm_values[] = {
  4032, 2016, 1008,
  504, 756, 252
};

/* Distinct hertz frequencies */
static const uint16_t tone_values[] = {
  0, 554,
  494, 466, 415, 369, 311, 277,
  247, 233, 185, 138, 92
};

/* All prescaler options */
static const uint8_t prescalers[]= {0x00, 0x03, 0x06, 0x08, 0x0a, 0x00};

static void get_prescaler(uint32_t a_freq,
                          uint8_t *a_ocr,
                          uint8_t *a_prescaler) {
  uint16_t ocr = 0x00;
  uint8_t prescaler = 0x00;

  /* Find the correct prescaler */
  do {
    ocr = (uint16_t)
      (F_CPU / ((a_freq << 1) * (0x01 << prescalers[prescaler])));
    ++prescaler;
  } while ((ocr > 255) && (prescalers[prescaler]));

  --ocr;
  if (ocr > 255) ocr = 255;

  *a_ocr = ocr & 0xff;
  *a_prescaler = prescaler;
}

static void play(uint16_t tone) {
  if (tone == 0) {
    DDRD &= ~_BV(PORTD5);
    PORTD &= ~_BV(PORTD5);
  } else {
    DDRD |= _BV(PORTD5);
  }

  uint8_t prescaler = 0x00,
    ocr = 0x00;

  get_prescaler(tone, &ocr, &prescaler);
  OCR0B = ocr;
  TCNT0 = 0x00;

  TCCR0B &= ~7;
  TCCR0B |= prescaler & 0x07;
}

static bool processed_note = false;
static uint8_t note_index = 0;

static uint16_t get_rhythm_value(rhythm_t r) {
  return rhythm_values[r];
}

static uint16_t get_tone_value(tone_t t) {
  return tone_values[t];
}

void pre_init_music() {
  /* ensure that compare B has a value */
  OCR0B = 0xFF;
  /* Turn off pin 5 to filter noise */
  DDRD &= ~_BV(PORTD5);
  PORTD &= ~_BV(PORTD5);
}

void post_init_music() {
  /* Insert small pause at the beginning */
  set_timeout(&processed_note, get_rhythm_value(r1));
}

static void get_note_values(uint16_t *t, uint16_t *r, uint8_t note) {
  *r = get_rhythm_value((rhythm_t) (note & 7));
  *t = get_tone_value((tone_t) (note >> 3));
}

void conduct() {
  if (processed_note) {
    uint16_t tone, rhythm;
    get_note_values(&tone, &rhythm, pgm_read_byte(boom_song + note_index));
    play(tone);
    set_timeout(&processed_note, rhythm);
    processed_note = false;
    if (++note_index == BOOM_SONG_SIZE) note_index = 0;
  }
}
