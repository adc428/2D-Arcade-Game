#include "Player.h"
#include <cstring>
#include <cctype>
#include "Obstacle.h"

Player::Player(const Point& startPos,
    const char(&the_keys)[NUM_KEYS + 1],
    Screen& theScreen)
    : pos(startPos), screen(&theScreen)
{
    std::memcpy(keys, the_keys, NUM_KEYS * sizeof(keys[0]));
    currentDir = Direction::STAY;
    pos.setDirection(currentDir);
    spring.reset();
}

void Player::draw() {
    pos.draw();
}

void Player::startSpringRelease() {
    spring.startRelease(currentDir);
    currentDir = spring.getReleaseDir();
    pos.setDirection(currentDir);
    spring.restoreChars(*screen, pos);
}

void Player::startExternalSpring(Direction dir, int speed, int ticksLeft) {
    spring.startExternal(dir, speed, ticksLeft);
    currentDir = dir;
    pos.setDirection(currentDir);
}

Player::MoveResult Player::moveAndCheck(int otherPlayerX, int otherPlayerY) {
    MoveResult result;
    Point oldPos = pos;

    if (spring.isInRelease()) {
        for (int step = 0; step < spring.getSpeed(); ++step) {
            Point next = pos;
            next.setDirection(spring.getReleaseDir());
            next.move();

            bool hitOther = (otherPlayerX >= 0 &&
                next.getX() == otherPlayerX &&
                next.getY() == otherPlayerY);

            if (hitOther) {
                result.hitOtherPlayer = true;
                result.hitAtX = otherPlayerX;
                result.hitAtY = otherPlayerY;
                break;
            }

            if (screen->isWall(next) || screen->isRiddle(next)) {
                spring.reset();
                pos.setDirection(currentDir);
                break;
            }

            if (screen->isObstacle(next)) {
                int force = spring.getSpeed();
                if (!Obstacle::tryPush(*screen, next, spring.getReleaseDir(), force)) {
                    spring.reset();
                    pos.setDirection(currentDir);
                    break;
                }
            }

            pos = next;

            if (screen->isKey(pos) && !hasItem()) {
                setItem('K');
                screen->setCharAt(pos, ' ');
            }
            // BOMB PICKUP REMOVED - handled by Game::handleBombPickup
        }

        if (!spring.tick()) {
            pos.setDirection(currentDir);
        }
    }
    else {
        pos.move();
        Point newPos = pos;

        if (oldPos.getX() == newPos.getX() && oldPos.getY() == newPos.getY()) {
            // STAY
        }
        else {
            bool newIsWall = screen->isWall(newPos);
            bool newIsObstacle = screen->isObstacle(newPos);
            bool newIsRiddle = screen->isRiddle(newPos);
            bool oldIsSpring = screen->isSpring(oldPos);
            bool newIsSpring = screen->isSpring(newPos);

            if (newIsWall || newIsRiddle) {
                pos = oldPos;

                if (oldIsSpring && spring.getCompressedChars() > 0) {
                    startSpringRelease();
                }
                else {
                    spring.reset();
                }

                if (newIsRiddle) {
                    result.hitRiddle = true;
                    result.riddleX = newPos.getX();
                    result.riddleY = newPos.getY();
                }
            }
            else if (newIsObstacle) {
                if (oldIsSpring && spring.getCompressedChars() > 0) {
                    pos = oldPos;
                    startSpringRelease();
                }
                else {
                    int force = 1;
                    if (!Obstacle::tryPush(*screen, newPos, currentDir, force)) {
                        pos = oldPos;
                        spring.reset();
                    }
                }
            }
            else {
                if (oldIsSpring && newIsSpring) {
                    spring.addCollapsedPosition(oldPos.getX(), oldPos.getY());
                    screen->setCharAt(oldPos, ' ');
                    spring.compress();
                }
                else if (!oldIsSpring && newIsSpring) {
                    spring.reset();
                    spring.compress();
                }
                else if (oldIsSpring && !newIsSpring && !spring.isInRelease()) {
                    spring.reset();
                }

                if (screen->isKey(newPos) && !hasItem()) {
                    setItem('K');
                    screen->setCharAt(newPos, ' ');
                }
                // BOMB PICKUP REMOVED - handled by Game::handleBombPickup
            }
        }
    }

    char underOld = screen->getCharAt(oldPos);
    oldPos.draw(underOld);
    pos.draw();

    return result;
}

void Player::handleKeyPressed(char key_pressed) {
    size_t index = 0;
    for (char k : keys) {
        if (std::tolower(static_cast<unsigned char>(k)) ==
            std::tolower(static_cast<unsigned char>(key_pressed))) {

            if (spring.isInRelease()) {
                if (index < 5) {
                    Direction newDir = (Direction)index;
                    if (newDir == Direction::STAY) return;
                    if (newDir == opposite(spring.getReleaseDir())) return;
                    if (newDir == spring.getReleaseDir()) return;
                    currentDir = newDir;
                    pos.setDirection(currentDir);
                }
                return;
            }

            if (index < 5) {
                Direction newDir = (Direction)index;

                if (spring.getCompressedChars() > 0 && screen->isSpring(pos)) {
                    if (newDir == Direction::STAY || newDir != currentDir) {
                        startSpringRelease();
                        return;
                    }
                }

                currentDir = newDir;
                pos.setDirection(currentDir);
            }
            else {
                // =============================================================
                // Dispose item (key 5 = E for player1, O for player2)
                // ORIGINAL WORKING CODE - saves item to screen board
                // =============================================================
                if (!hasItem()) return;

                char underMe = screen->getCharAt(pos);
                if (underMe != ' ') return;

                screen->setCharAt(pos, getItem());
                clearItem();
            }

            return;
        }
        ++index;
    }
}