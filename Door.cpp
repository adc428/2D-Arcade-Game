#include "Door.h"

// Door ID N requires N keys to open
int getRequiredKeysForDoor(int doorId) {
    if (doorId < 1) return 0;
    if (doorId > 9) doorId = 9;

    return doorId;
}