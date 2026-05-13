#include "Spring.h"
#include "Screen.h"
#include "Point.h"

void SpringState::reset() {
    inRelease = false;
    releaseDir = Direction::STAY;
    speed = 0;
    ticksLeft = 0;
    compressedChars = 0;
    collapsedCount = 0;
}

void SpringState::compress() {
    compressedChars++;
}

void SpringState::addCollapsedPosition(int x, int y) {
    if (collapsedCount < MAX_COMPRESSED) {
        collapsedX[collapsedCount] = x;
        collapsedY[collapsedCount] = y;
        collapsedCount++;
    }
}

// =============================================================================
// Source: AI Generated (Claude)
// Prompt: "Start spring release in opposite direction"
// =============================================================================
void SpringState::startRelease(Direction currentMoveDir) {
    if (compressedChars == 0) {
        reset();
        return;
    }

    inRelease = true;
    releaseDir = opposite(currentMoveDir);
    speed = compressedChars;
    ticksLeft = speed * speed;
}

void SpringState::startExternal(Direction dir, int spd, int ticks) {
    inRelease = true;
    releaseDir = dir;
    speed = spd;
    ticksLeft = ticks;
    compressedChars = 0;
    collapsedCount = 0;
}

bool SpringState::tick() {
    if (ticksLeft > 0) {
        ticksLeft--;
    }

    if (ticksLeft == 0) {
        reset();
        return false;
    }

    return true;
}

// =============================================================================
// Source: AI Generated (Claude)
// Prompt: "Restore collapsed springs from saved positions"
// =============================================================================
void SpringState::restoreChars(Screen& screen, const Point& playerPos) {
    (void)playerPos;

    for (int i = 0; i < collapsedCount; ++i) {
        Point springPos(collapsedX[i], collapsedY[i], 0, 0, '#');
        screen.setCharAt(springPos, '#');
        springPos.draw('#');
    }

    collapsedCount = 0;
}