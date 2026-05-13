#pragma once

#include "Screen.h"
#include "Player.h"
#include "Riddle.h"
#include "Switch.h"
#include "Bomb.h"

class Game {
private:
    static constexpr int NUM_ROOMS = 3;
    static constexpr int INITIAL_LIVES = 3;

    Screen rooms[NUM_ROOMS];
    int currentRoom = 0;

    Player player1;
    Player player2;

    // Lives system - shared between both players
    int lives = INITIAL_LIVES;

    // Door system
    int doorKeysNeeded[10] = { 0 };      // Keys still needed for each door
    int doorSwitchesNeeded[10] = { 0 };  // Switches still needed for each door
    char doorIdMap[NUM_ROOMS][Screen::MAX_Y][Screen::MAX_X] = { 0 };
    bool doorReady[10] = { false };

    // Player state
    bool playerWaiting[2] = { false, false };

    // Door animation
    bool blinkState = false;
    int blinkTick = 0;

    // Riddle system
    static constexpr int MAX_RIDDLES = 10;
    Riddle riddles[MAX_RIDDLES];
    int riddleCount = 0;

    // Switch system
    SwitchManager switchManager;

    // Bomb system
    BombManager bombManager;

public:
    Game();
    void run();

private:
    void startNewGame();
    void gameLoop();
    void drawPlayersStatus() const;
    void drawPlayersInCurrentRoom();

    bool loadScreensFromFiles();

    // Door system
    void scanDoorsAndInit();
    void updateDoorDisplay(int doorId);
    bool useKeyOnDoor(Player& player);
    int getDoorIdAt(int x, int y) const;
    void handleDoorTransition(Player& player);
    void transitionToNextRoom();
    void animateOpenDoors();

    int getPlayerIndex(const Player& player) const;

    // Riddle system
    void initRiddles();
    void scanAndInitRiddles();
    bool handleRiddle(Player& player, const Point& riddlePos);
    void showRiddlePopup(const Riddle& riddle, int& chosenAnswer);

    // Joint pushing
    bool arePlayersAdjacent(const Player& p1, const Player& p2) const;

    // Lives system
    void loseLife();
    void showGameOver();
    void restartGame();

    // Switch system
    void handleSwitch(Player& player);

    // Bomb system
    void handleBombPickup(Player& player);
    void updateBombs();
};