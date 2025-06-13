#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "game.h"
#include "globals.h"

ISR(PORTB_PORT_vect);

Game game;

int main(void) {
  // sei();
  game.gameLoop();
}

ISR(PORTB_PORT_vect) {
    uint8_t flags = PORTB.INTFLAGS;
    PORTB.INTFLAGS = 0xFF;

    game.inputCapture = flags;
}
