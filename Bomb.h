#pragma once

class Screen;

class BombManager {
public:
    static constexpr int MAX_BOMBS = 10;
    static constexpr char BOMB_CHAR = '@';
    static constexpr int FUSE_TICKS = 5;
    static constexpr int BLINK_TICKS = 13;
    static constexpr int EXPLOSION_RADIUS = 3;

private:
    struct BombInfo {
        int x = -1;
        int y = -1;
        int room = -1;
        int fuseTicksLeft = 0;
        int blinkTicksLeft = 0;
        bool active = false;
        bool blinking = false;
    };

    BombInfo bombs[MAX_BOMBS];
    int bombCount = 0;

public:
    BombManager() = default;

    bool placeBomb(int x, int y, int room);
    int update(Screen& screen, int currentRoom);
    bool processExplosion(Screen& screen, int bombX, int bombY,
        int p1X, int p1Y, int p2X, int p2Y,
        bool& p1Hit, bool& p2Hit);
    bool getBombInfo(int index, int& x, int& y, int& room) const;
    void deactivateBomb(int index);
    void reset();

    // Check if there's an active (placed, ticking) bomb at position
    bool isActiveBombAt(int x, int y, int room) const;
};