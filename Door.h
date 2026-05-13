#pragma once

// Returns how many keys are needed to open a door with given ID
// Door ID N requires N keys (e.g., door '3' needs 3 keys)
int getRequiredKeysForDoor(int doorId);