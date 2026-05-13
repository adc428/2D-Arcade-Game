#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include "Point.h"

using std::cout, std::endl;
using std::string;

class Screen {
public:
    enum { MAX_X = 80, MAX_Y = 22 };

private:
    char board[MAX_Y][MAX_X + 1];
    int levelIndex;
    int legendX;
    int legendY;

public:
    Screen(int level = 0)
        : levelIndex(level), legendX(0), legendY(22)
    {
        reset();
    }

    bool loadFromFile(const string& filename);
    void reset();

    char getCharAt(const Point& p) const {
        return board[p.getY()][p.getX()];
    }

    char getCharAt(int x, int y) const {
        if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y) return ' ';
        return board[y][x];
    }

    void setCharAt(const Point& p, char c) {
        board[p.getY()][p.getX()] = c;
    }

    void setCharAt(int x, int y, char c) {
        if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
            board[y][x] = c;
        }
    }

    void draw() const;

    bool isWall(const Point& p) const { return getCharAt(p) == 'W'; }
    bool isKey(const Point& p) const { return getCharAt(p) == 'K'; }
    bool isDoor(const Point& p) const {
        char c = getCharAt(p);
        return (c >= '1' && c <= '9');
    }
    bool isSpring(const Point& p) const { return getCharAt(p) == '#'; }
    bool isObstacle(const Point& p) const { return getCharAt(p) == '*'; }
    bool isRiddle(const Point& p) const { return getCharAt(p) == '?'; }
    bool isSwitch(const Point& p) const {
        char c = getCharAt(p);
        return (c == '/' || c == '\\');
    }
    bool isBomb(const Point& p) const { return getCharAt(p) == '@'; }

    bool findDoor(char doorId, int& outX, int& outY) const;

    int getLegendX() const { return legendX; }
    int getLegendY() const { return legendY; }
};