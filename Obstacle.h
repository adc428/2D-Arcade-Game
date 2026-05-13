#pragma once

#include "Screen.h"
#include "Direction.h"
#include "Point.h"

class Obstacle {
public:
    // Attempts to push obstacle(s) starting at 'start' position in direction 'dir'
    // Returns true if push succeeded, false if blocked
    // availableForce determines how many connected obstacles can be pushed
    static bool tryPush(Screen& screen,
        const Point& start,
        Direction dir,
        int availableForce);
};