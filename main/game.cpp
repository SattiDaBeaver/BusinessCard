#include "game.h"
#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

#define TEST

Game::Game(){
    numRows = 4;
    numColumns = 5;
    minGapSize = 1;
    playerRow = 1;
    score = 0;
    inputCapture = 0;

    gameOver = 0;

    srand(TCB0.CNT);

    initializeGrid();
    setupInput();
}

void Game::initializeGrid(void) {
    for (int row = 0; row < this->numRows; row++){
        for (int col = 0; col < this->numColumns; col++){
            #ifndef TEST
                this->grid[row][col] = 0;
            #else
                if (row == playerRow && col == 0){
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

    PORTB.PIN0CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTB.PIN1CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTB.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;

    PORTB.INTFLAGS = 0x0F;  // Clear PB0–PB3 flags
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
        if (regVal != 0){
            clearLED();
        
            setPinOutReg(regVal | (1 << (col + 1)));
            setPinHigh(col + 1);

            setPinLowReg(regVal);
        }

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

void Game::gameLoop(void){
    int delay = 0;
    int maxCount = 1000;

    int wangdapudu = 0;
    int maxWangdapudu = 3;

    while (1){
        if (gameOver){
            restartGame();
        }

        delay = (delay + 1) % maxCount;

        if (delay == 0 || delay == (maxCount >> 1)){
            if (getInput() & (IN_UP | IN_LEFT)){
                movePlayer(DIR_UP);
            }
            if (getInput() & (IN_DOWN | IN_RIGHT)){
                movePlayer(DIR_DOWN);
            }
        }

    
        if (delay == 0){
            wangdapudu = (wangdapudu + 1) % maxWangdapudu;

            if (wangdapudu == 0){
                shiftLane(OBSTACLE);
            }
            else {
                shiftLane(NO_OBSTACLE);
            }
        }

        gameOver = detectCollision();

        plexLine();
    }
}

void Game::getRandomObstacle(uint8_t minGapSize, uint8_t* laneArray){
    uint8_t gapSize = (rand() % 3) + minGapSize;
    if (gapSize >= numRows){
        gapSize = numRows - 1;
    }
    uint8_t position = (rand() % (5 - gapSize));
    for (int row = 0; row < numRows; row++){
        if (row >= position && row < (position + gapSize)){
            laneArray[row] = 0;
        }
        else {
            laneArray[row] = 1;
        }
    }
}

void Game::shiftLane(uint8_t newLane){
    for (int col = 1; col < numColumns; col++){
        for (int row = 0; row < numRows; row++){
            if (col == 1){
                grid[row][col] = 0; // Clear the first column
            }
            else {
                grid[row][col - 1] = grid[row][col]; // Shift left
            }
        }
    }

    if (newLane == 0){
        for (int row = 0; row < numRows; row++){
            grid[row][numColumns - 1] = 0;
        }
        return;
    }

    uint8_t laneArray[4];
    getRandomObstacle(minGapSize, laneArray);
    for (int row = 0; row < numRows; row++){
        grid[row][numColumns - 1] = laneArray[row];
    }
    return;
}

void Game::movePlayer(int dir){
    playerRow += dir;
    if (playerRow >= numRows){
        playerRow = 0;
    }
    else if (playerRow < 0){
        playerRow = numRows - 1;
    }
    for (int row = 0; row < numRows; row++){
        if (row == playerRow){
            this->grid[row][0] = 1;
        }
        else {
            this->grid[row][0] = 0;
        }
    }
}

bool Game::detectCollision(void){
    if (grid[playerRow][0] && grid[playerRow][1]){
        return 1;
    }
    return 0;
}

void Game::restartGame(void){
    int count = 0;
    int maxCount = 30000;
    int wait = 0;
    int maxWait = 4;
    while(1){
        count++;
        if (count >= maxCount){
            count = 0;
            wait = (wait + 1) % maxWait;
        }
        if (count < (maxCount >> 5)){
            plexLine();
        } else {
            clearLED();
        }

        if (getInput() && (wait == maxWait - 1)){
            minGapSize = 1;
            playerRow = 1;
            score = 0;
            inputCapture = 0;

            gameOver = 0;

            initializeGrid();
            return;
        }
    }
}


