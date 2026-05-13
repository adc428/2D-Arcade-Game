#include "Switch.h"
#include "Screen.h"
#include "utils.h"
#include <iostream>

// =============================================================================
// Scan rooms for switches and link each to the door in its room
// IMPORTANT: This must be called BEFORE scanDoorsAndInit!
// =============================================================================
void SwitchManager::scanAndInit(Screen rooms[], int numRooms) {
    switchCount = 0;

    // First pass: find all switches AND link to doors in same room
    for (int r = 0; r < numRooms; ++r) {
        // First, find what door ID is in this room
        int doorIdInRoom = 0;
        for (int y = 0; y < Screen::MAX_Y && doorIdInRoom == 0; ++y) {
            for (int x = 0; x < Screen::MAX_X && doorIdInRoom == 0; ++x) {
                char c = rooms[r].getCharAt(x, y);
                if (c >= '1' && c <= '9') {
                    doorIdInRoom = c - '0';
                }
            }
        }

        // Now find all switches in this room and link them to the door
        for (int y = 0; y < Screen::MAX_Y; ++y) {
            for (int x = 0; x < Screen::MAX_X; ++x) {
                char c = rooms[r].getCharAt(x, y);

                if (c == SWITCH_ON || c == SWITCH_OFF) {
                    if (switchCount < MAX_SWITCHES) {
                        switches[switchCount].x = x;
                        switches[switchCount].y = y;
                        switches[switchCount].room = r;
                        switches[switchCount].isOn = (c == SWITCH_ON);
                        switches[switchCount].wasToggled = (c == SWITCH_ON);
                        switches[switchCount].linkedDoorId = doorIdInRoom;  // Link to door in this room
                        switches[switchCount].exists = true;
                        switchCount++;
                    }
                }
            }
        }
    }
}

int SwitchManager::findSwitchAt(int x, int y, int room) const {
    for (int i = 0; i < switchCount; ++i) {
        if (switches[i].exists &&
            switches[i].x == x &&
            switches[i].y == y &&
            switches[i].room == room) {
            return i;
        }
    }
    return -1;
}

// =============================================================================
// Toggle switch - can only toggle OFF->ON, not back
// =============================================================================
bool SwitchManager::toggleSwitch(int index, Screen& screen) {
    if (index < 0 || index >= switchCount) return false;
    if (!switches[index].exists) return false;

    // If already toggled ON, cannot toggle again
    if (switches[index].wasToggled && switches[index].isOn) {
        return false;
    }

    // Toggle to ON
    switches[index].isOn = true;
    switches[index].wasToggled = true;

    // Update display on screen
    int x = switches[index].x;
    int y = switches[index].y;

    screen.setCharAt(x, y, SWITCH_ON);

    gotoxy(x, y);
    std::cout << SWITCH_ON;
    std::cout.flush();

    return true;
}

int SwitchManager::countOnSwitchesForDoor(int doorId) const {
    int count = 0;
    for (int i = 0; i < switchCount; ++i) {
        if (switches[i].exists &&
            switches[i].linkedDoorId == doorId &&
            switches[i].isOn) {
            count++;
        }
    }
    return count;
}

int SwitchManager::countTotalSwitchesForDoor(int doorId) const {
    int count = 0;
    for (int i = 0; i < switchCount; ++i) {
        if (switches[i].exists && switches[i].linkedDoorId == doorId) {
            count++;
        }
    }
    return count;
}

void SwitchManager::reset() {
    for (int i = 0; i < MAX_SWITCHES; ++i) {
        switches[i] = SwitchInfo{};
    }
    switchCount = 0;
}