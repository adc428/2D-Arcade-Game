#include "Screen.h"
#include "utils.h"
#include <fstream>

void Screen::draw() const {
    cls();
    gotoxy(0, 0);
    for (int y = 0; y < MAX_Y; ++y) {
        cout << board[y] << endl;
    }
    cout.flush();
}

bool Screen::findDoor(char doorId, int& outX, int& outY) const {
    for (int y = 0; y < MAX_Y; ++y) {
        for (int x = 0; x < MAX_X; ++x) {
            if (board[y][x] == doorId) {
                outX = x;
                outY = y;
                return true;
            }
        }
    }
    return false;
}

// =============================================================================
// Source: AI Generated (Claude)
// Prompt: "Load screen from file, find Legend position from 'L' character"
// =============================================================================
bool Screen::loadFromFile(const string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    // Initialize with spaces
    for (int y = 0; y < MAX_Y; ++y) {
        for (int x = 0; x < MAX_X; ++x) {
            board[y][x] = ' ';
        }
        board[y][MAX_X] = '\0';
    }

    legendX = 0;
    legendY = 22;

    string line;
    int y = 0;

    while (y < MAX_Y && std::getline(file, line)) {
        int x = 0;
        for (char ch : line) {
            if (x >= MAX_X) break;

            if (ch == 'L') {
                legendX = x;
                legendY = y;
                board[y][x] = ' ';
            }
            else {
                board[y][x] = ch;
            }
            ++x;
        }

        while (x < MAX_X) {
            board[y][x] = ' ';
            ++x;
        }

        board[y][MAX_X] = '\0';
        ++y;
    }

    while (y < MAX_Y) {
        for (int x = 0; x < MAX_X; ++x) {
            board[y][x] = ' ';
        }
        board[y][MAX_X] = '\0';
        ++y;
    }

    file.close();
    return true;
}

void Screen::reset() {
    for (int y = 0; y < MAX_Y; ++y) {
        for (int x = 0; x < MAX_X; ++x) {
            if (y == 0 || y == MAX_Y - 1 || x == 0 || x == MAX_X - 1) {
                board[y][x] = 'W';
            }
            else {
                board[y][x] = ' ';
            }
        }
        board[y][MAX_X] = '\0';
    }

    legendX = 0;
    legendY = 22;
}