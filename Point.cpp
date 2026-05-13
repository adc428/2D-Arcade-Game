#include "Point.h"
#include "Screen.h"

// Moves point by diff_x/diff_y with screen wrapping
void Point::move() {
	x = (x + diff_x + Screen::MAX_X) % Screen::MAX_X;
	y = (y + diff_y + Screen::MAX_Y) % Screen::MAX_Y;
}

// Sets movement delta based on direction
void Point::setDirection(Direction dir) {
	switch (dir) {
	case Direction::UP:
		diff_x = 0;
		diff_y = -1;
		break;
	case Direction::RIGHT:
		diff_x = 1;
		diff_y = 0;
		break;
	case Direction::DOWN:
		diff_x = 0;
		diff_y = 1;
		break;
	case Direction::LEFT:
		diff_x = -1;
		diff_y = 0;
		break;
	case Direction::STAY:
		diff_x = 0;
		diff_y = 0;
		break;
	}
}

void Point::setPosition(int x1, int y1) {
	x = x1;
	y = y1;
}