#include <avr/eeprom.h>
#include "game.h"

static const uint8_t *highscores_magic_byte_offset = (uint8_t*)42;
static const uint16_t highscores_magic_byte_value = 42;

static const uint16_t highscores_data_offset = 43;
static const uint16_t highscores_data_size = sizeof(highscores_t);

static bool highscores_exist() {
  return (eeprom_read_byte((uint8_t*)highscores_magic_byte_offset)
          == highscores_magic_byte_value);
}

bool highscores_read(highscores_t highscores) {
  if (!highscores_exist()) return false;

  eeprom_read_block(highscores,
                    (void*)highscores_data_offset,
                    highscores_data_size);
  return true;
}

static void highscores_write(highscores_t highscores) {
  if (!highscores_exist()) {
    eeprom_update_byte((uint8_t*)highscores_magic_byte_offset,
                       highscores_magic_byte_value);
  }

  eeprom_write_block(highscores,
                     (void*)highscores_data_offset,
                     highscores_data_size);
}

static void highscores_insert_sorted(highscores_t highscores, uint16_t score) {
  uint16_t i;
  uint16_t next_score = 0;
  uint16_t next_score_temp = 0;

  for (i = 0; i < HIGHSCORE_COUNT; i++) {
    if (next_score != 0) {
      next_score_temp = highscores[i];
      highscores[i] = next_score;
      next_score = next_score_temp;
    } else if (highscores[i] < score) {
      next_score = highscores[i];
      highscores[i] = score;
    }
  }
}

bool highscores_add(uint16_t score) {
  highscores_t highscores = {0};
  highscores_read(highscores);
  if (highscores[HIGHSCORE_COUNT - 1] != 0
      || highscores[HIGHSCORE_COUNT - 1] > score) {
    return false;
  }

  highscores_insert_sorted(highscores, score);

  highscores_write(highscores);
  return true;
}
