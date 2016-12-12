#include <avr/io.h>
#include <pleasant-lcd.h>
#include "game.h"

#define PIN1_DDR DDRC
#define PIN1_PORT PORTC
#define PIN1_NUM PINC0

#define PIN2_DDR DDRC
#define PIN2_PORT PORTC
#define PIN2_NUM PINC1


void init_seven_segment() {
	// Set data direction for ports
	PIN1_DDR |= (1 << PIN1_NUM);
	PIN2_DDR |= (1 << PIN2_NUM);
}

void render_player_to_seven_segment(player_t *player) {
	if (get_player_flag(player, PLAYER_FLAG_HEALTH_UPDATED)) {
		uint8_t lives = player->lives;

		// Set output to pins
		if (lives & 1) {
			PIN1_PORT |= (1 << PIN1_NUM);
		} else {
			PIN1_PORT &= ~(1 << PIN1_NUM);
		}

		if (lives & 2) {
			PIN2_PORT |= (1 << PIN2_NUM);
		} else {
			PIN2_PORT &= ~(1 << PIN2_NUM);
		}
	}
}
