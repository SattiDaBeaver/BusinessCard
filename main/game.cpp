#include "game.h"
#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

#define TEST

Game::Game(){
    numRows = 4;
    numColumns = 5;
    initializeGrid();
    setupInput();
}

void Game::initializeGrid(void) {
    for (int row = 0; row < this->numRows; row++){
        for (int col = 0; col < this->numColumns; col++){
            #ifndef TEST
                this->grid[row][col] = 0;
            #else
                if (row == 0 || col == 0 || row == numRows - 1 || col == numColumns - 1){
                    this->grid[row][col] = 1;
                }
                else {
                    this->grid[row][col] = 0;
                }
            #endif
        }
    }
}

void Game::setupInput(void){
    PORTB.DIRCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;  // Set PB0–PB3 as input

    PORTB.PIN0CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN3CTRL = PORT_PULLUPEN_bm;
}

uint8_t Game::getInput(void){
    uint8_t input = ((~PORTB.IN) & 0x0F);
    return input;
}

void Game::plexPixel(void){
    uint8_t pixelCount = 0;
    for (int i = 1; i <= OUTPUT_PIN_NUM; i++){
        for (int j = 1; j <= OUTPUT_PIN_NUM; j++){
            pixelCount++;
            if (i == j){
                continue;
            }

            //if (grid[pixelCount % 5][pixelCount / 5] != 0){
                setPinOutClear(i);
                setPinOut(j);
                setPinHigh(i);
                setPinLow(j);
            //}
            
            // softDelay(30000);
        }
    }
}

void Game::plexLine(void){
    for (int col = 0; col < numColumns; col++){
        uint8_t regVal = 0;

        for (int row = 0; row < numRows; row++){
            if (grid[row][col] != 0){
                if (row < col){
                    regVal |= (1 << (row + 1));
                }
                else {
                    regVal |= (1 << (row + 2));
                }
            }
        }

        clearLED();
        
        setPinOutReg(regVal | (1 << (col + 1)));
        setPinHigh(col + 1);

        setPinLowReg(regVal);

        // softDelay(10);
    }
}

void Game::clearLED(void){
    PORTA.OUT = 0x00; // Set all LOW
    PORTA.DIR = 0x00; // Set all as input
}


void Game::setPinOut(uint8_t pin1){
    PORTA.DIR |= (1 << pin1);
}

void Game::setPinOutClear(uint8_t pin1){
    PORTA.DIR = (1 << pin1);
}

void Game::setPinOutReg(uint8_t regVal){
    PORTA.DIR |= regVal;
}


void Game::setPinLow(uint8_t pin1){
    PORTA.OUT &= ~(1 << pin1);
}

void Game::setPinLowReg(uint8_t regVal){
    PORTA.OUT &= ~regVal;
}


void Game::setPinHigh(uint8_t pin1){
    PORTA.OUT |= (1 << pin1);
}

void Game::setPinHighReg(uint8_t regVal){
    PORTA.OUT |= regVal;
}


void Game::softDelay(uint32_t delay){
    while (delay) {
        __asm__("nop\n\t");
        delay--;
    }
}

// Game Logic
uint8_t Game::getRandomObstacle(uint8_t minGapSize){
    uint8_t gapSize = (rand() % 3) + minGapSize;

}


