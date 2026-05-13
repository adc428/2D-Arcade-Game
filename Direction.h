#pragma once

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    STAY
};

// Returns the opposite direction
inline Direction opposite(Direction dir) {
    switch (dir) {
    case Direction::UP:    return Direction::DOWN;
    case Direction::DOWN:  return Direction::UP;
    case Direction::LEFT:  return Direction::RIGHT;
    case Direction::RIGHT: return Direction::LEFT;
    case Direction::STAY:  return Direction::STAY;
    }
    return Direction::STAY;
}