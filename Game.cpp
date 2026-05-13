#include "Game.h"
#include "utils.h"
#include "Obstacle.h"
#include "Door.h"
#include "GameUI.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>

enum Keys { ESC = 27 };

struct RoomSpawn {
    int p1x, p1y;
    int p2x, p2y;
};

static RoomSpawn getRoomSpawn(int roomIndex) {
    switch (roomIndex) {
    case 0:  return { 1, 1, Screen::MAX_X - 2, 1 };
    case 1:  return { 2, 17, 2, 13 };
    default: return { Screen::MAX_X / 2 - 2, Screen::MAX_Y / 2,
                      Screen::MAX_X / 2 + 2, Screen::MAX_Y / 2 };
    }
}

Game::Game()
    : rooms{ Screen(0), Screen(1), Screen(2) }
    , currentRoom(0)
    , player1(Point(10, 10, 0, 0, '$'), "wdxase", rooms[currentRoom])
    , player2(Point(15, 10, 0, 0, '&'), "ilmjko", rooms[currentRoom])
    , lives(INITIAL_LIVES)
{
}

bool Game::arePlayersAdjacent(const Player& p1, const Player& p2) const {
    int dx = p1.getPos().getX() - p2.getPos().getX();
    int dy = p1.getPos().getY() - p2.getPos().getY();
    return (dx == 0 && (dy == 1 || dy == -1)) || (dy == 0 && (dx == 1 || dx == -1));
}

// =============================================================================
// Lives System
// =============================================================================
void Game::loseLife() {
    lives--;

    if (lives <= 0) {
        showGameOver();
    }
    else {
        const int msgX = 20, msgY = 10;
        gotoxy(msgX, msgY);
        std::cout << "+------------------------------------------+";
        gotoxy(msgX, msgY + 1);
        std::cout << "|           You lost a life!               |";
        gotoxy(msgX, msgY + 2);
        std::cout << "|       Lives remaining: " << lives << "                  |";
        gotoxy(msgX, msgY + 3);
        std::cout << "+------------------------------------------+";

        Sleep(1500);
        rooms[currentRoom].draw();
        drawPlayersInCurrentRoom();
        drawPlayersStatus();
    }
}

void Game::showGameOver() {
    cls();
    const int startX = 20, startY = 8;

    gotoxy(startX, startY);
    std::cout << "+------------------------------------------+";
    gotoxy(startX, startY + 1);
    std::cout << "|            *** GAME OVER ***             |";
    gotoxy(startX, startY + 2);
    std::cout << "|       You ran out of lives!              |";
    gotoxy(startX, startY + 3);
    std::cout << "|   Press any key to restart...            |";
    gotoxy(startX, startY + 4);
    std::cout << "+------------------------------------------+";

    std::cout.flush();
    _getch();
    restartGame();
}

void Game::restartGame() {
    currentRoom = 0;
    lives = INITIAL_LIVES;

    loadScreensFromFiles();

    RoomSpawn spawn = getRoomSpawn(currentRoom);
    player1 = Player(Point(spawn.p1x, spawn.p1y, 0, 0, '$'), "wdxase", rooms[currentRoom]);
    player2 = Player(Point(spawn.p2x, spawn.p2y, 0, 0, '&'), "ilmjko", rooms[currentRoom]);

    switchManager.scanAndInit(rooms, NUM_ROOMS);
    scanDoorsAndInit();
    bombManager.reset();
    initRiddles();
    scanAndInitRiddles();

    blinkTick = 0;
    blinkState = false;
    playerWaiting[0] = false;
    playerWaiting[1] = false;

    rooms[currentRoom].draw();
    drawPlayersInCurrentRoom();
    drawPlayersStatus();
}

// =============================================================================
// Switch System - Switch acts like a KEY (decrements door number by 1)
// =============================================================================
void Game::handleSwitch(Player& player) {
    const Point& pos = player.getPos();
    int switchIndex = switchManager.findSwitchAt(pos.getX(), pos.getY(), currentRoom);

    if (switchIndex >= 0) {
        int linkedDoorId = switchManager.getLinkedDoorId(switchIndex);

        if (switchManager.toggleSwitch(switchIndex, rooms[currentRoom])) {
            // Switch was toggled ON - acts like using a key!
            if (linkedDoorId > 0 && linkedDoorId <= 9) {
                // Decrement keys needed (switch acts as key)
                if (doorKeysNeeded[linkedDoorId] > 0) {
                    doorKeysNeeded[linkedDoorId]--;
                }

                // Check if door should open
                if (doorKeysNeeded[linkedDoorId] <= 0) {
                    doorReady[linkedDoorId] = true;
                }

                updateDoorDisplay(linkedDoorId);
            }
        }
    }
}

// =============================================================================
// Bomb System - with active bomb protection
// =============================================================================
void Game::handleBombPickup(Player& player) {
    const Point& pos = player.getPos();
    int px = pos.getX();
    int py = pos.getY();

    // Don't pick up if there's an ACTIVE bomb here (placed and ticking)
    if (bombManager.isActiveBombAt(px, py, currentRoom)) {
        return;
    }

    // Pick up bomb from screen
    if (rooms[currentRoom].isBomb(pos) && !player.hasItem()) {
        player.setItem('@');
        rooms[currentRoom].setCharAt(pos, ' ');
    }
}

void Game::updateBombs() {
    int explodedIndex = bombManager.update(rooms[currentRoom], currentRoom);

    if (explodedIndex >= 0) {
        int bombX, bombY, bombRoom;
        if (bombManager.getBombInfo(explodedIndex, bombX, bombY, bombRoom)) {
            if (bombRoom == currentRoom) {
                bool p1Hit = false, p2Hit = false;
                bombManager.processExplosion(
                    rooms[currentRoom], bombX, bombY,
                    player1.getPos().getX(), player1.getPos().getY(),
                    player2.getPos().getX(), player2.getPos().getY(),
                    p1Hit, p2Hit
                );

                bombManager.deactivateBomb(explodedIndex);

                rooms[currentRoom].draw();
                drawPlayersInCurrentRoom();
                drawPlayersStatus();

                if (p1Hit || p2Hit) {
                    loseLife();
                }
            }
            else {
                bombManager.deactivateBomb(explodedIndex);
            }
        }
    }
}

// =============================================================================
// Game Loop
// =============================================================================
void Game::gameLoop() {
    bool backToMenu = false;

    while (!backToMenu && currentRoom >= 0 && lives > 0) {

        if (_kbhit()) {
            char key = _getch();

            if (key == Keys::ESC) {
                const int startX = 5, startY = 8;
                gotoxy(startX, startY);
                std::cout << "+--------------------------------------------------------------------+";
                gotoxy(startX, startY + 1);
                std::cout << "|                            GAME PAUSED                             |";
                gotoxy(startX, startY + 2);
                std::cout << "|     Press ESC to continue, or H to return to main menu             |";
                gotoxy(startX, startY + 3);
                std::cout << "+--------------------------------------------------------------------+";

                bool waitInPause = true;
                while (waitInPause) {
                    char c = _getch();
                    if (c == Keys::ESC) {
                        rooms[currentRoom].draw();
                        drawPlayersStatus();
                        animateOpenDoors();
                        waitInPause = false;
                    }
                    else if (c == 'H' || c == 'h') {
                        backToMenu = true;
                        waitInPause = false;
                    }
                }
            }
            else {
                // =============================================================
                // Track bomb state BEFORE handleKeyPressed
                // =============================================================
                bool p1HadBomb = player1.hasItem() && player1.getItem() == '@';
                bool p2HadBomb = player2.hasItem() && player2.getItem() == '@';
                Point p1PosBefore = player1.getPos();
                Point p2PosBefore = player2.getPos();

                // Handle key press
                if (!playerWaiting[0] && player1.getScreen() == &rooms[currentRoom])
                    player1.handleKeyPressed(key);
                if (!playerWaiting[1] && player2.getScreen() == &rooms[currentRoom])
                    player2.handleKeyPressed(key);

                // =============================================================
                // Register bomb with BombManager IMMEDIATELY after drop
                // This ensures isActiveBombAt returns true before pickup check
                // =============================================================
                if (p1HadBomb && !player1.hasItem()) {
                    bombManager.placeBomb(p1PosBefore.getX(), p1PosBefore.getY(), currentRoom);
                }
                if (p2HadBomb && !player2.hasItem()) {
                    bombManager.placeBomb(p2PosBefore.getX(), p2PosBefore.getY(), currentRoom);
                }
            }
        }

        if (backToMenu || currentRoom < 0 || lives <= 0) break;

        bool active1 = (!playerWaiting[0] && player1.getScreen() == &rooms[currentRoom]);
        bool active2 = (!playerWaiting[1] && player2.getScreen() == &rooms[currentRoom]);

        Point pos1Before = player1.getPos();
        Point pos2Before = player2.getPos();

        // Joint pushing
        if (active1 && active2 && arePlayersAdjacent(player1, player2) &&
            !player1.isInSpringRelease() && !player2.isInSpringRelease()) {

            if (player1.getDirection() == player2.getDirection() &&
                player1.getDirection() != Direction::STAY) {

                Direction pushDir = player1.getDirection();
                int dx = 0, dy = 0;
                switch (pushDir) {
                case Direction::UP:    dy = -1; break;
                case Direction::DOWN:  dy = 1;  break;
                case Direction::LEFT:  dx = -1; break;
                case Direction::RIGHT: dx = 1;  break;
                default: break;
                }

                int p1Proj = player1.getPos().getX() * dx + player1.getPos().getY() * dy;
                int p2Proj = player2.getPos().getX() * dx + player2.getPos().getY() * dy;
                const Player& frontPlayer = (p1Proj > p2Proj) ? player1 : player2;

                Point obstaclePos(frontPlayer.getPos().getX() + dx,
                    frontPlayer.getPos().getY() + dy, 0, 0, ' ');

                if (rooms[currentRoom].isObstacle(obstaclePos)) {
                    int totalForce = player1.getForce() + player2.getForce();
                    Obstacle::tryPush(rooms[currentRoom], obstaclePos, pushDir, totalForce);
                }
            }
        }

        // Spring collision
        bool springCollisionHandled = false;
        if (active1 && active2) {
            auto isTargetAdjacent = [&](Player& attacker, const Point& attackerPos, const Point& targetPos) -> bool {
                if (!attacker.isInSpringRelease()) return false;
                Direction dir = attacker.getSpringReleaseDir();
                int dx = 0, dy = 0;
                switch (dir) {
                case Direction::UP:    dy = -1; break;
                case Direction::DOWN:  dy = 1;  break;
                case Direction::LEFT:  dx = -1; break;
                case Direction::RIGHT: dx = 1;  break;
                default: return false;
                }
                return (attackerPos.getX() + dx == targetPos.getX() &&
                    attackerPos.getY() + dy == targetPos.getY());
                };

            if (player1.isInSpringRelease() && !player2.isInSpringRelease() &&
                isTargetAdjacent(player1, pos1Before, pos2Before)) {
                springCollisionHandled = true;
                player2.startExternalSpring(player1.getSpringReleaseDir(),
                    player1.getSpringSpeed(),
                    player1.getSpringTicksLeft());
            }
            else if (player2.isInSpringRelease() && !player1.isInSpringRelease() &&
                isTargetAdjacent(player2, pos2Before, pos1Before)) {
                springCollisionHandled = true;
                player1.startExternalSpring(player2.getSpringReleaseDir(),
                    player2.getSpringSpeed(),
                    player2.getSpringTicksLeft());
            }
        }

        // Move players
        if (!springCollisionHandled) {
            if (active1) {
                char savedChar2 = 0;
                bool blocked2 = false;
                if (active2 && !player1.isInSpringRelease()) {
                    savedChar2 = rooms[currentRoom].getCharAt(pos2Before);
                    rooms[currentRoom].setCharAt(pos2Before, 'W');
                    blocked2 = true;
                }

                Player::MoveResult res1 = player1.moveAndCheck(
                    active2 ? pos2Before.getX() : -1,
                    active2 ? pos2Before.getY() : -1);

                if (res1.hitRiddle) {
                    Point riddlePos(res1.riddleX, res1.riddleY, 0, 0, '?');
                    if (handleRiddle(player1, riddlePos)) {
                        player1.setPosition(res1.riddleX, res1.riddleY);
                        player1.draw();
                    }
                    else {
                        player1.stop();
                        loseLife();
                        if (lives <= 0) break;
                    }
                }

                handleSwitch(player1);
                handleBombPickup(player1);
                handleDoorTransition(player1);

                if (blocked2) rooms[currentRoom].setCharAt(pos2Before, savedChar2);
            }

            if (active2) {
                Point pos1Now = player1.getPos();
                char savedChar1 = 0;
                bool blocked1 = false;
                if (active1 && !player2.isInSpringRelease()) {
                    savedChar1 = rooms[currentRoom].getCharAt(pos1Now);
                    rooms[currentRoom].setCharAt(pos1Now, 'W');
                    blocked1 = true;
                }

                Player::MoveResult res2 = player2.moveAndCheck(
                    active1 ? pos1Now.getX() : -1,
                    active1 ? pos1Now.getY() : -1);

                if (res2.hitRiddle) {
                    Point riddlePos(res2.riddleX, res2.riddleY, 0, 0, '?');
                    if (handleRiddle(player2, riddlePos)) {
                        player2.setPosition(res2.riddleX, res2.riddleY);
                        player2.draw();
                    }
                    else {
                        player2.stop();
                        loseLife();
                        if (lives <= 0) break;
                    }
                }

                handleSwitch(player2);
                handleBombPickup(player2);
                handleDoorTransition(player2);

                if (blocked1) rooms[currentRoom].setCharAt(pos1Now, savedChar1);
            }
        }

        // Collision check
        if (active1 && active2) {
            Point finalPos1 = player1.getPos();
            Point finalPos2 = player2.getPos();
            if (finalPos1.getX() == finalPos2.getX() && finalPos1.getY() == finalPos2.getY()) {
                char under = rooms[currentRoom].getCharAt(finalPos1);
                finalPos1.draw(under);
                player2.setPosition(pos2Before.getX(), pos2Before.getY());
                player1.draw();
                player2.draw();
            }
        }

        updateBombs();
        drawPlayersStatus();
        animateOpenDoors();
        Sleep(150);
    }
}

void Game::run() {
    hideCursor();
    bool shouldExit = false;

    while (!shouldExit) {
        char choice = GameUI::showMainMenuAndGetChoice();

        switch (choice) {
        case '1': startNewGame(); break;
        case '8': GameUI::showInstructions(); break;
        case '9': shouldExit = true; break;
        }
    }

    cls();
}

bool Game::loadScreensFromFiles() {
    namespace fs = std::filesystem;
    std::vector<std::string> screenFiles;

    try {
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename.find("adv-world") == 0 && filename.find(".screen") != std::string::npos) {
                    screenFiles.push_back(filename);
                }
            }
        }
    }
    catch (...) { return false; }

    std::sort(screenFiles.begin(), screenFiles.end());
    if (screenFiles.empty()) return false;

    int loaded = 0;
    for (int i = 0; i < (int)screenFiles.size() && loaded < NUM_ROOMS; ++i) {
        if (rooms[loaded].loadFromFile(screenFiles[i])) ++loaded;
    }

    if (loaded < NUM_ROOMS) {
        for (int i = 0; i < NUM_ROOMS; ++i) rooms[i].reset();
        return false;
    }

    return true;
}

void Game::startNewGame() {
    if (!loadScreensFromFiles()) {
        for (int i = 0; i < NUM_ROOMS; ++i) rooms[i].reset();
    }

    currentRoom = 0;
    lives = INITIAL_LIVES;

    RoomSpawn spawn = getRoomSpawn(currentRoom);
    player1 = Player(Point(spawn.p1x, spawn.p1y, 0, 0, '$'), "wdxase", rooms[currentRoom]);
    player2 = Player(Point(spawn.p2x, spawn.p2y, 0, 0, '&'), "ilmjko", rooms[currentRoom]);

    // Initialize switches BEFORE doors
    switchManager.scanAndInit(rooms, NUM_ROOMS);
    scanDoorsAndInit();
    bombManager.reset();
    initRiddles();
    scanAndInitRiddles();

    blinkTick = 0;
    blinkState = false;
    playerWaiting[0] = false;
    playerWaiting[1] = false;

    rooms[currentRoom].draw();
    drawPlayersInCurrentRoom();
    drawPlayersStatus();

    gameLoop();
}

void Game::drawPlayersInCurrentRoom() {
    if (!playerWaiting[0] && player1.getScreen() == &rooms[currentRoom]) player1.draw();
    if (!playerWaiting[1] && player2.getScreen() == &rooms[currentRoom]) player2.draw();
}

void Game::drawPlayersStatus() const {
    const int infoY = rooms[currentRoom].getLegendY();
    const int infoX = rooms[currentRoom].getLegendX();

    std::string line1 = "Lives: " + std::to_string(lives) + " | Room: " + std::to_string(currentRoom + 1);
    line1 += " | P1: ";
    if (player1.hasItem()) {
        line1 += (player1.getItem() == 'K') ? "Key" : (player1.getItem() == '@') ? "Bomb" : std::string(1, player1.getItem());
    }
    else line1 += "Empty";
    line1 += " | P2: ";
    if (player2.hasItem()) {
        line1 += (player2.getItem() == 'K') ? "Key" : (player2.getItem() == '@') ? "Bomb" : std::string(1, player2.getItem());
    }
    else line1 += "Empty";

    while ((int)line1.size() < Screen::MAX_X) line1 += ' ';

    std::string line2 = "P1($): (" + std::to_string(player1.getPos().getX()) + "," +
        std::to_string(player1.getPos().getY()) + ") | P2(&): (" +
        std::to_string(player2.getPos().getX()) + "," +
        std::to_string(player2.getPos().getY()) + ")";
    while ((int)line2.size() < Screen::MAX_X) line2 += ' ';

    std::string line3(64, '-');
    while ((int)line3.size() < Screen::MAX_X) line3 += ' ';

    gotoxy(infoX, infoY);     std::cout << line1;
    gotoxy(infoX, infoY + 1); std::cout << line2;
    gotoxy(infoX, infoY + 2); std::cout << line3;
}

// =============================================================================
// Riddle System
// =============================================================================
void Game::initRiddles() {
    std::ifstream file("riddles.txt");
    riddleCount = 0;
    if (!file.is_open()) return;

    std::string line, currentQuestion, currentAnswers;

    while (std::getline(file, line) && riddleCount < MAX_RIDDLES) {
        if (line.empty()) continue;

        if (line.find("---") == 0) {
            if (!currentQuestion.empty() && !currentAnswers.empty()) {
                std::vector<std::string> answers;
                size_t start = 0, comma = currentAnswers.find(',');

                while (comma != std::string::npos) {
                    std::string answer = currentAnswers.substr(start, comma - start);
                    answer.erase(0, answer.find_first_not_of(" \t\r\n"));
                    answer.erase(answer.find_last_not_of(" \t\r\n") + 1);
                    answers.push_back(answer);
                    start = comma + 1;
                    comma = currentAnswers.find(',', start);
                }
                std::string lastAnswer = currentAnswers.substr(start);
                lastAnswer.erase(0, lastAnswer.find_first_not_of(" \t\r\n"));
                lastAnswer.erase(lastAnswer.find_last_not_of(" \t\r\n") + 1);
                answers.push_back(lastAnswer);

                if (!answers.empty()) {
                    std::string opts[3] = {
                        answers.size() > 0 ? answers[0] : "",
                        answers.size() > 1 ? answers[1] : "",
                        answers.size() > 2 ? answers[2] : ""
                    };
                    riddles[riddleCount++] = Riddle(currentQuestion, opts, 0, 0, 0);
                }
                currentQuestion.clear();
                currentAnswers.clear();
            }
            continue;
        }

        if (currentQuestion.empty()) currentQuestion = line;
        else if (currentAnswers.empty()) currentAnswers = line;
    }

    file.close();
}

void Game::scanAndInitRiddles() {
    int riddleIdx = 0;

    for (int room = 0; room < NUM_ROOMS && riddleIdx < riddleCount; ++room) {
        for (int y = 0; y < Screen::MAX_Y && riddleIdx < riddleCount; ++y) {
            for (int x = 0; x < Screen::MAX_X && riddleIdx < riddleCount; ++x) {
                if (rooms[room].getCharAt(x, y) == '?') {
                    std::string opts[3] = {
                        riddles[riddleIdx].getOption(0),
                        riddles[riddleIdx].getOption(1),
                        riddles[riddleIdx].getOption(2)
                    };
                    riddles[riddleIdx] = Riddle(riddles[riddleIdx].getQuestion(), opts, 0, x, y);
                    riddleIdx++;
                }
            }
        }
    }
}

bool Game::handleRiddle(Player& player, const Point& riddlePos) {
    if (rooms[currentRoom].getCharAt(riddlePos) != '?') return true;

    int riddleIndex = -1;
    for (int i = 0; i < riddleCount; ++i) {
        if (riddles[i].getX() == riddlePos.getX() &&
            riddles[i].getY() == riddlePos.getY() &&
            !riddles[i].isSolved()) {
            riddleIndex = i;
            break;
        }
    }

    if (riddleIndex == -1) return true;

    int chosenAnswer = -1;
    showRiddlePopup(riddles[riddleIndex], chosenAnswer);

    if (riddles[riddleIndex].checkAnswer(chosenAnswer)) {
        riddles[riddleIndex].markSolved();
        rooms[currentRoom].setCharAt(riddlePos, ' ');
        return true;
    }
    else {
        gotoxy(25, 10);
        std::cout << "+--------------------------------+";
        gotoxy(25, 11);
        std::cout << "|     Wrong answer! Try again    |";
        gotoxy(25, 12);
        std::cout << "+--------------------------------+";
        Sleep(1000);
        rooms[currentRoom].draw();
        drawPlayersStatus();
        return false;
    }
}

void Game::showRiddlePopup(const Riddle& riddle, int& chosenAnswer) {
    const int boxWidth = 70, boxHeight = 10;
    const int startX = (Screen::MAX_X - boxWidth) / 2;
    const int startY = (Screen::MAX_Y - boxHeight) / 2;

    gotoxy(startX, startY);
    std::cout << '+' << std::string(boxWidth - 2, '-') << '+';

    for (int i = 1; i < boxHeight - 1; ++i) {
        gotoxy(startX, startY + i);
        std::cout << '|' << std::string(boxWidth - 2, ' ') << '|';
    }

    gotoxy(startX, startY + boxHeight - 1);
    std::cout << '+' << std::string(boxWidth - 2, '-') << '+';

    gotoxy(startX + 2, startY + 1); std::cout << "RIDDLE:";
    gotoxy(startX + 2, startY + 2); std::cout << riddle.getQuestion();
    gotoxy(startX + 2, startY + 4); std::cout << "1. " << riddle.getOption(0);
    gotoxy(startX + 2, startY + 5); std::cout << "2. " << riddle.getOption(1);
    gotoxy(startX + 2, startY + 6); std::cout << "3. " << riddle.getOption(2);
    gotoxy(startX + 2, startY + 8); std::cout << "Choose your answer (1-3): ";

    char input = 0;
    while (input < '1' || input > '3') input = _getch();
    chosenAnswer = input - '1';

    rooms[currentRoom].draw();
    drawPlayersStatus();
}