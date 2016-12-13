#include <avr/io.h>
#include <stdio.h>
#include "game.h"

/* We need to divide the 10-bit output of the ADC to a number between 0 and
   100. That means we need to divide the output by 10.24, but because that's
   not an integer conversion we use 11. */
#define BRIGHTNESS_CONTROL_DIVISOR 11

void init_brightness_control() {
  ADMUX = 0;
  ADCSRA = 0;
  ADCSRB = 0;

  ADMUX |= (1 << REFS0);        /* Internal reference */
  ADMUX |= (1 << MUX0)
    | (1 << MUX1);              /* ADC3 */
  ADCSRA |= (1 << ADPS0)
    | (1 << ADPS1)
    | (1 << ADPS2);             /* Prescaler 128 */
  ADCSRA |= (1 << ADEN);        /* Enable the ADC */

  ADCSRA |= (1 << ADATE);       /* Auto trigger */
  ADCSRA |= (1 << ADSC);        /* Start the first conversion */
}

uint8_t brightness_control_brightness() {
  return ADC / BRIGHTNESS_CONTROL_DIVISOR;
}
