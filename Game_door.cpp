#include "Game.h"
#include "utils.h"
#include "Obstacle.h"
#include "Door.h"
#include "GameUI.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <string>

// =============================================================================
// Door System Implementation
// =============================================================================

void Game::scanDoorsAndInit()
{
    // Clear all door data
    for (int d = 0; d <= 9; ++d) {
        doorKeysNeeded[d] = 0;
        doorSwitchesNeeded[d] = 0;
        doorReady[d] = false;
    }

    // Clear door ID map
    for (int r = 0; r < NUM_ROOMS; ++r)
        for (int y = 0; y < Screen::MAX_Y; ++y)
            for (int x = 0; x < Screen::MAX_X; ++x)
                doorIdMap[r][y][x] = 0;

    // Scan all screens for door characters
    for (int r = 0; r < NUM_ROOMS; ++r) {
        for (int y = 0; y < Screen::MAX_Y; ++y) {
            for (int x = 0; x < Screen::MAX_X; ++x) {
                char c = rooms[r].getCharAt(x, y);
                if (c >= '1' && c <= '9') {
                    int id = c - '0';
                    doorIdMap[r][y][x] = c;  // Store ORIGINAL door ID

                    // Door number = total keys/switches needed
                    // Set only once per door ID
                    if (doorKeysNeeded[id] == 0) {
                        doorKeysNeeded[id] = id;
                    }
                }
            }
        }
    }

    // doorSwitchesNeeded is not used - switches act like keys
}

void Game::updateDoorDisplay(int doorId)
{
    int keysRemaining = doorKeysNeeded[doorId];
    char shown;

    if (doorReady[doorId]) {
        shown = '1';
    }
    else if (keysRemaining <= 0) {
        doorReady[doorId] = true;
        shown = '1';
    }
    else if (keysRemaining > 9) {
        shown = '9';
    }
    else {
        shown = '0' + keysRemaining;
    }

    char idChar = '0' + doorId;

    for (int r = 0; r < NUM_ROOMS; ++r) {
        for (int y = 0; y < Screen::MAX_Y; ++y) {
            for (int x = 0; x < Screen::MAX_X; ++x) {
                if (doorIdMap[r][y][x] == idChar) {
                    rooms[r].setCharAt(x, y, shown);
                    if (r == currentRoom) {
                        gotoxy(x, y);
                        std::cout << shown;
                    }
                }
            }
        }
    }
}

bool Game::useKeyOnDoor(Player& player)
{
    if (!player.hasItem() || player.getItem() != 'K')
        return false;

    const Point& p = player.getPos();
    char idChar = doorIdMap[currentRoom][p.getY()][p.getX()];
    if (idChar == 0)
        return false;

    int doorId = idChar - '0';

    if (doorReady[doorId])
        return false;
    if (doorKeysNeeded[doorId] <= 0)
        return false;

    player.clearItem();
    doorKeysNeeded[doorId]--;

    if (doorKeysNeeded[doorId] <= 0) {
        doorReady[doorId] = true;
    }

    updateDoorDisplay(doorId);
    return true;
}

int Game::getDoorIdAt(int x, int y) const
{
    char idChar = doorIdMap[currentRoom][y][x];
    if (idChar == 0) return 0;
    return idChar - '0';
}

int Game::getPlayerIndex(const Player& player) const
{
    if (&player == &player1) return 0;
    if (&player == &player2) return 1;
    return -1;
}

void Game::handleDoorTransition(Player& player)
{
    useKeyOnDoor(player);

    const Point& pos = player.getPos();
    int doorId = getDoorIdAt(pos.getX(), pos.getY());

    if (doorId != 0 && doorReady[doorId]) {
        int idx = getPlayerIndex(player);
        if (idx >= 0 && !playerWaiting[idx]) {
            playerWaiting[idx] = true;

            if (playerWaiting[0] && playerWaiting[1]) {
                transitionToNextRoom();
            }
        }
    }
}

void Game::transitionToNextRoom()
{
    int nextRoom = currentRoom + 1;

    if (nextRoom >= NUM_ROOMS) {
        GameUI::showWinScreen();
        currentRoom = -1;
        playerWaiting[0] = false;
        playerWaiting[1] = false;
        return;
    }

    currentRoom = nextRoom;
    playerWaiting[0] = false;
    playerWaiting[1] = false;

    int p1x, p1y, p2x, p2y;
    if (currentRoom == 0) {
        p1x = 1;                  p1y = 1;
        p2x = Screen::MAX_X - 2;  p2y = 1;
    }
    else if (currentRoom == 1) {
        p1x = 2;   p1y = 17;
        p2x = 2;   p2y = 13;
    }
    else {
        p1x = Screen::MAX_X / 2 - 2;  p1y = Screen::MAX_Y / 2;
        p2x = Screen::MAX_X / 2 + 2;  p2y = Screen::MAX_Y / 2;
    }

    player1.setScreen(rooms[currentRoom]);
    player2.setScreen(rooms[currentRoom]);
    player1.setPosition(p1x, p1y);
    player2.setPosition(p2x, p2y);
    player1.stop();
    player2.stop();

    cls();
    rooms[currentRoom].draw();
    drawPlayersInCurrentRoom();
    drawPlayersStatus();
}

void Game::animateOpenDoors()
{
    ++blinkTick;
    if (blinkTick % 2 == 0)
        blinkState = !blinkState;

    for (int doorId = 1; doorId <= 9; ++doorId) {
        if (!doorReady[doorId]) continue;

        char idChar = char('0' + doorId);

        for (int y = 0; y < Screen::MAX_Y; ++y) {
            for (int x = 0; x < Screen::MAX_X; ++x) {
                if (doorIdMap[currentRoom][y][x] == idChar) {
                    gotoxy(x, y);
                    std::cout << (blinkState ? '1' : ' ');
                }
            }
        }
    }

    drawPlayersInCurrentRoom();
}