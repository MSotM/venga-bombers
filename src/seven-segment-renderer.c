#include <avr/io.h>
#include "game.h"

void init_seven_segment() {
  /* Set data direction for ports */
  DDRC |= _BV(PORTC0);
  DDRC |= _BV(PORTC1);
}

void render_to_seven_segment(uint8_t number) {
  /* Set output to pins */
  if (number & 1) {
    PORTC |= _BV(PORTC0);
  } else {
    PORTC &= _BV(PORTC0);
  }

  if (number & 2) {
    PORTC |= _BV(PORTC1);
  } else {
    PORTC &= _BV(PORTC1);
  }
}
