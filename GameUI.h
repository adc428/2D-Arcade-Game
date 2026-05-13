#pragma once

#include "utils.h"
#include <conio.h>
#include <Windows.h>
#include <iostream>

// Central place for all text-based UI screens (menus, instructions, win screen).
class GameUI {
public:
    // Shows the main menu and returns the key the user chose.
    static char showMainMenuAndGetChoice();

    // Shows instructions / controls / legend.
    static void showInstructions();

    // Shows the final "you win" screen.
    static void showWinScreen();
};
