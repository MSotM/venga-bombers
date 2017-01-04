#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

#define BOOM_SONG_SIZE 239
#define NOTE(t, r) ((t << 3) | r)

void pre_init_music();
void post_init_music();

typedef uint8_t note_t;

extern const note_t boom_song[];

/*
 * A rhythm symbol stands for the division of the left and right number.
 * When there is only one number it's a whole number i.e. 'r1' is equal to
 * four crotchets
 */
typedef enum {
  r1, r1_2, r1_4,
  r1_8, r3_16, r1_16
} rhythm_t;

/*
 * There are only a few tones used for the song. The corresponding tone values
 * represent the hertz frequency of the tone.
 */
typedef enum {
  rest, cis5,
  b4, ais4, gis4, fis4, dis4, cis4,
  b3, ais3, fis3, cis3, fis2
} tone_t;

/*
 * The conductor checks whether the time has come to change its note. This
 * function must be called repeatedly by the main thread in order to
 * function properly.
 */
void conduct();

#endif /* AUDIO_H */
