#ifndef GAME_H
#define GAME_H  

#include <stdint.h>

class Game {
    public:
        char grid[4][5]; // 4 Rows, 5 Columns
        uint8_t numRows, numColumns;
        uint8_t minGapSize;
        int playerRow;
        int score;
        volatile uint8_t inputCapture;
        bool gameOver;
        Game();

        // Setup
        void initializeGrid(void);
        void setupInput(void);


        // Display Control and Input
        void clearLED(void);

        void setPinOut(uint8_t pin1);
        void setPinOutClear(uint8_t pin1);
        void setPinOutReg(uint8_t regVal);

        void setPinLow(uint8_t pin1);
        void setPinLowReg(uint8_t regVal);

        void setPinHigh(uint8_t pin1);
        void setPinHighReg(uint8_t regVal);

        void softDelay(uint32_t delay);
    
        uint8_t getInput(void);

        void plexPixel(void);
        void plexLine(void);

        // Game Logic
        void gameLoop(void);
        void getRandomObstacle(uint8_t minGapSize, uint8_t* laneArray);
        void shiftLane(uint8_t newLane);
        void movePlayer(int dir);
        bool detectCollision(void);
        void restartGame(void);
};

#endif