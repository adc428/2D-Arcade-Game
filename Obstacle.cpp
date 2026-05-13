#include "Obstacle.h"
#include <iostream>

struct Cell {
    int x;
    int y;
};

// Attempts to push a chain of obstacles in the given direction
// Returns true if successful, false if blocked or insufficient force
bool Obstacle::tryPush(Screen& screen,
    const Point& start,
    Direction dir,
    int availableForce)
{
    if (availableForce <= 0) {
        return false;
    }

    // Verify starting position is an obstacle
    if (screen.getCharAt(start) != '*') {
        return false;
    }

    // Calculate direction delta
    int dx = 0, dy = 0;
    switch (dir) {
    case Direction::UP:    dy = -1; break;
    case Direction::DOWN:  dy = 1;  break;
    case Direction::LEFT:  dx = -1; break;
    case Direction::RIGHT: dx = 1;  break;
    default:
        return false;
    }

    const int W = Screen::MAX_X;
    const int H = Screen::MAX_Y;

    // Find all connected obstacles in push direction
    Cell cells[Screen::MAX_X * Screen::MAX_Y];
    int cellsCount = 0;

    int x = start.getX();
    int y = start.getY();

    while (x >= 0 && x < W && y >= 0 && y < H) {
        Point p(x, y, 0, 0, ' ');
        char ch = screen.getCharAt(p);

        if (ch != '*')
            break;

        cells[cellsCount].x = x;
        cells[cellsCount].y = y;
        ++cellsCount;

        x += dx;
        y += dy;
    }

    if (cellsCount == 0) {
        return false;
    }

    // Check if force is sufficient to push all obstacles
    int size = cellsCount;
    if (availableForce < size) {
        return false;
    }

    // Check if destination is valid (empty space)
    int nextX = x;
    int nextY = y;

    if (nextX < 0 || nextX >= W || nextY < 0 || nextY >= H) {
        return false;
    }

    Point nextP(nextX, nextY, 0, 0, ' ');
    char nextCh = screen.getCharAt(nextP);

    if (nextCh != ' ') {
        return false;
    }

    // Clear old positions
    for (int i = 0; i < cellsCount; ++i) {
        Point p(cells[i].x, cells[i].y, 0, 0, ' ');
        screen.setCharAt(p, ' ');
        p.draw(' ');
    }

    // Draw at new positions
    for (int i = 0; i < cellsCount; ++i) {
        Point p(cells[i].x + dx, cells[i].y + dy, 0, 0, '*');
        screen.setCharAt(p, '*');
        p.draw('*');
    }

    return true;
}