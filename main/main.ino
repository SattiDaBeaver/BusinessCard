#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "game.h"
#include "globals.h"

int main(void) {
  // PORTA.DIR = (1 << PIN4) | (1 << PIN5);  // Set as output

  // PORTA.OUT &= ~(1 << PIN4);
  // // PORTA.OUT |= (1 << PIN5);

  // // PORTA.OUT &= ~(1 << PIN2);
  // // PORTA.OUT |= (1 << PIN3);

  Game game;
  int count = 0;
  int maxCount = 1000;

  while (1) {
    count = (count + 1) % maxCount;
    if (game.getInput()) {
      game.clearLED();
    }
    else {
      game.plexLine();
      if (count == 0){
        game.shiftLane();
      }
      
      // game.plexPixel();
    }
  }
}
