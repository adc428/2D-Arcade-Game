#pragma once
#include "Point.h"
#include "Screen.h"
#include "Spring.h"

class Player {
    static constexpr int NUM_KEYS = 6;
    Point pos;
    char keys[NUM_KEYS];       // Control keys for this player
    Screen* screen;
    char item = ' ';           // Currently held item (' ' = empty, 'K' = key)

    Direction currentDir = Direction::STAY;
    SpringState spring;

public:
    Player(const Point& point, const char(&the_keys)[NUM_KEYS + 1], Screen& theScreen);

    void stop() {
        currentDir = Direction::STAY;
        pos.setDirection(currentDir);
    }
    void draw();
    void handleKeyPressed(char key);

    // Inventory
    bool hasItem() const { return item != ' '; }
    char getItem() const { return item; }
    void setItem(char c) { item = c; }
    void clearItem() { item = ' '; }

    // Position and direction
    const Point& getPos() const { return pos; }
    void setPosition(int x, int y) { pos.setPosition(x, y); }
    void setScreen(Screen& s) { screen = &s; }
    Screen* getScreen() const { return screen; }
    Direction getDirection() const { return currentDir; }

    // Force: returns 1 normally, or spring speed if in spring release
    int getForce() const {
        return spring.isInRelease() ? spring.getSpeed() : 1;
    }

    // Spring state accessors
    bool isInSpringRelease() const { return spring.isInRelease(); }
    Direction getSpringReleaseDir() const { return spring.getReleaseDir(); }
    int getSpringSpeed() const { return spring.getSpeed(); }
    int getSpringTicksLeft() const { return spring.getTicksLeft(); }
    void tickSpring() { spring.tick(); }

    void startExternalSpring(Direction dir, int speed, int ticksLeft);

    // Result of movement attempt
    struct MoveResult {
        bool hitOtherPlayer = false;
        int hitAtX = -1;
        int hitAtY = -1;

        bool hitRiddle = false;
        int riddleX = -1;
        int riddleY = -1;
    };

    MoveResult moveAndCheck(int otherPlayerX, int otherPlayerY);

private:
    void startSpringRelease();
};