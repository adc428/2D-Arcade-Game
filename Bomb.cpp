#include "Bomb.h"
#include "Screen.h"
#include "utils.h"
#include <iostream>
#include <cmath>

bool BombManager::placeBomb(int x, int y, int room) {
    for (int i = 0; i < MAX_BOMBS; ++i) {
        if (!bombs[i].active) {
            bombs[i].x = x;
            bombs[i].y = y;
            bombs[i].room = room;
            bombs[i].fuseTicksLeft = FUSE_TICKS;
            bombs[i].blinkTicksLeft = BLINK_TICKS;
            bombs[i].active = true;
            bombs[i].blinking = false;
            if (i >= bombCount) bombCount = i + 1;
            return true;
        }
    }
    return false;
}

int BombManager::update(Screen& screen, int currentRoom) {
    for (int i = 0; i < bombCount; ++i) {
        if (!bombs[i].active) continue;

        bool inCurrentRoom = (bombs[i].room == currentRoom);

        if (!bombs[i].blinking) {
            bombs[i].fuseTicksLeft--;
            if (bombs[i].fuseTicksLeft <= 0) {
                bombs[i].blinking = true;
            }
        }
        else {
            if (inCurrentRoom) {
                char displayChar = (bombs[i].blinkTicksLeft % 2 == 0) ? BOMB_CHAR : ' ';
                gotoxy(bombs[i].x, bombs[i].y);
                std::cout << displayChar;
                std::cout.flush();
            }

            bombs[i].blinkTicksLeft--;

            if (bombs[i].blinkTicksLeft <= 0) {
                if (inCurrentRoom) {
                    gotoxy(bombs[i].x, bombs[i].y);
                    std::cout << '*';
                    std::cout.flush();
                }
                return i;
            }
        }
    }
    return -1;
}

bool BombManager::processExplosion(Screen& screen, int bombX, int bombY,
    int p1X, int p1Y, int p2X, int p2Y,
    bool& p1Hit, bool& p2Hit) {
    p1Hit = false;
    p2Hit = false;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int checkX = bombX + dx;
            int checkY = bombY + dy;
            if (checkX < 0 || checkX >= Screen::MAX_X || checkY < 0 || checkY >= Screen::MAX_Y) continue;
            if (screen.getCharAt(checkX, checkY) == 'W') {
                screen.setCharAt(checkX, checkY, ' ');
                gotoxy(checkX, checkY);
                std::cout << ' ';
            }
        }
    }

    screen.setCharAt(bombX, bombY, ' ');
    gotoxy(bombX, bombY);
    std::cout << ' ';

    for (int dy = -EXPLOSION_RADIUS; dy <= EXPLOSION_RADIUS; ++dy) {
        for (int dx = -EXPLOSION_RADIUS; dx <= EXPLOSION_RADIUS; ++dx) {
            if (abs(dx) + abs(dy) > EXPLOSION_RADIUS) continue;
            int checkX = bombX + dx;
            int checkY = bombY + dy;
            if (checkX < 0 || checkX >= Screen::MAX_X || checkY < 0 || checkY >= Screen::MAX_Y) continue;
            char c = screen.getCharAt(checkX, checkY);
            if (c == '*' || c == 'K' || c == '?' || c == '#' || c == '@') {
                screen.setCharAt(checkX, checkY, ' ');
                gotoxy(checkX, checkY);
                std::cout << ' ';
            }
        }
    }

    if (abs(p1X - bombX) + abs(p1Y - bombY) <= EXPLOSION_RADIUS) p1Hit = true;
    if (abs(p2X - bombX) + abs(p2Y - bombY) <= EXPLOSION_RADIUS) p2Hit = true;

    return p1Hit || p2Hit;
}

bool BombManager::getBombInfo(int index, int& x, int& y, int& room) const {
    if (index < 0 || index >= MAX_BOMBS) return false;
    x = bombs[index].x;
    y = bombs[index].y;
    room = bombs[index].room;
    return bombs[index].active;
}

void BombManager::deactivateBomb(int index) {
    if (index >= 0 && index < MAX_BOMBS) {
        bombs[index].active = false;
        bombs[index].blinking = false;
    }
}

void BombManager::reset() {
    for (int i = 0; i < MAX_BOMBS; ++i) bombs[i] = BombInfo{};
    bombCount = 0;
}

bool BombManager::isActiveBombAt(int x, int y, int room) const {
    for (int i = 0; i < bombCount; ++i) {
        if (bombs[i].active &&
            bombs[i].x == x &&
            bombs[i].y == y &&
            bombs[i].room == room) {
            return true;
        }
    }
    return false;
}