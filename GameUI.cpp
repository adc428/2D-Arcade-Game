#include "GameUI.h"

char GameUI::showMainMenuAndGetChoice()
{
    cls();

    const int startX = 5;
    const int startY = 2;

    gotoxy(startX, startY);
    std::cout << "+--------------------------------------------------------------+";

    gotoxy(startX, startY + 1);
    std::cout << "|                       ***** MAIN MENU *****                  |";

    gotoxy(startX, startY + 2);
    std::cout << "|                                                              |";

    gotoxy(startX, startY + 3);
    std::cout << "|  Two players are trapped in mysterious puzzle rooms.         |";

    gotoxy(startX, startY + 4);
    std::cout << "|  Work TOGETHER to push obstacles, use springs, solve riddles |";

    gotoxy(startX, startY + 5);
    std::cout << "|  and open doors using keys. If one fails - both are stuck.   |";

    gotoxy(startX, startY + 6);
    std::cout << "|                                                              |";

    gotoxy(startX, startY + 7);
    std::cout << "|  (1)  START A NEW GAME                                       |";

    gotoxy(startX, startY + 8);
    std::cout << "|  (8)  INSTRUCTIONS AND KEYS                                  |";

    gotoxy(startX, startY + 9);
    std::cout << "|  (9)  EXIT                                                   |";

    gotoxy(startX, startY + 10);
    std::cout << "|                                                              |";

    gotoxy(startX, startY + 11);
    std::cout << "+--------------------------------------------------------------+";

    gotoxy(startX + 2, startY + 13);
    std::cout << "Choose option: ";

    char choice = _getch();
    std::cout << choice << std::endl;

    return choice;
}

void GameUI::showInstructions()
{
    cls();

    gotoxy(25, 0);
    std::cout << "*** INSTRUCTIONS ***";

    const int leftX = 4;
    const int baseY = 2;

    gotoxy(leftX, baseY);
    std::cout << "+-----------------+-----------+-----------+";
    gotoxy(leftX, baseY + 1);
    std::cout << "|     ACTION      |  P1 ( $ ) |  P2 ( & ) |";
    gotoxy(leftX, baseY + 2);
    std::cout << "+-----------------+-----------+-----------+";
    gotoxy(leftX, baseY + 3);
    std::cout << "| MOVE RIGHT      |     D     |     L     |";
    gotoxy(leftX, baseY + 4);
    std::cout << "| MOVE DOWN       |     X     |     M     |";
    gotoxy(leftX, baseY + 5);
    std::cout << "| MOVE LEFT       |     A     |     J     |";
    gotoxy(leftX, baseY + 6);
    std::cout << "| MOVE UP         |     W     |     I     |";
    gotoxy(leftX, baseY + 7);
    std::cout << "| STAY IN PLACE   |     S     |     K     |";
    gotoxy(leftX, baseY + 8);
    std::cout << "| DISPOSE ITEM    |     E     |     O     |";
    gotoxy(leftX, baseY + 9);
    std::cout << "+-----------------+-----------+-----------+";

    const int rightX = 48;

    gotoxy(rightX, baseY);
    std::cout << "+----------------+-------------+";
    gotoxy(rightX, baseY + 1);
    std::cout << "| Game Element   | Symbol      |";
    gotoxy(rightX, baseY + 2);
    std::cout << "+----------------+-------------+";
    gotoxy(rightX, baseY + 3);
    std::cout << "| Player 1       | $           |";
    gotoxy(rightX, baseY + 4);
    std::cout << "| Player 2       | &           |";
    gotoxy(rightX, baseY + 5);
    std::cout << "| Wall           | W           |";
    gotoxy(rightX, baseY + 6);
    std::cout << "| Obstacle       | *           |";
    gotoxy(rightX, baseY + 7);
    std::cout << "| Spring         | #           |";
    gotoxy(rightX, baseY + 8);
    std::cout << "| Switch On/Off  | /\\          |";
    gotoxy(rightX, baseY + 9);
    std::cout << "| Door           | 1-9         |";
    gotoxy(rightX, baseY + 10);
    std::cout << "| Key            | K           |";
    gotoxy(rightX, baseY + 11);
    std::cout << "| Riddle         | ?           |";
    gotoxy(rightX, baseY + 12);
    std::cout << "+----------------+-------------+";

    gotoxy(10, baseY + 15);
    std::cout << "Use cooperation, timing and communication to solve each room.";

    gotoxy(10, baseY + 17);
    std::cout << "Press any key to return to the main menu...";
    _getch();
}

void GameUI::showWinScreen()
{
    cls();

    const int startX = 15;
    const int startY = 7;

    gotoxy(startX, startY);
    std::cout << "+------------------------------------------------+";
    gotoxy(startX, startY + 1);
    std::cout << "|                                                |";
    gotoxy(startX, startY + 2);
    std::cout << "|       ***  CONGRATULATIONS!  ***               |";
    gotoxy(startX, startY + 3);
    std::cout << "|                                                |";
    gotoxy(startX, startY + 4);
    std::cout << "|     You have escaped the puzzle rooms!         |";
    gotoxy(startX, startY + 5);
    std::cout << "|          Freedom at last!                      |";
    gotoxy(startX, startY + 6);
    std::cout << "|                                                |";
    gotoxy(startX, startY + 7);
    std::cout << "|     Press any key to return to main menu...    |";
    gotoxy(startX, startY + 8);
    std::cout << "|                                                |";
    gotoxy(startX, startY + 9);
    std::cout << "+------------------------------------------------+";

    std::cout.flush();
    _getch();
}
