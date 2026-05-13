#pragma once
#include "Direction.h"

class Screen;
class Point;

// Manages spring compression and release state for a player
class SpringState {
    static constexpr int MAX_COMPRESSED = 10;

    bool inRelease = false;
    Direction releaseDir = Direction::STAY;
    int speed = 0;
    int ticksLeft = 0;
    int compressedChars = 0;

    // Store positions of collapsed springs for restoration
    int collapsedX[MAX_COMPRESSED];
    int collapsedY[MAX_COMPRESSED];
    int collapsedCount = 0;

public:
    SpringState() = default;

    bool isInRelease() const { return inRelease && ticksLeft > 0; }
    Direction getReleaseDir() const { return releaseDir; }
    int getSpeed() const { return speed; }
    int getTicksLeft() const { return ticksLeft; }
    int getCompressedChars() const { return compressedChars; }

    void reset();
    void compress();
    void addCollapsedPosition(int x, int y);
    void startRelease(Direction currentMoveDir);
    void startExternal(Direction dir, int spd, int ticks);
    bool tick();
    void restoreChars(Screen& screen, const Point& playerPos);
};