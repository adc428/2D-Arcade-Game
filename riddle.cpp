#include "Riddle.h"

Riddle::Riddle()
    : question(""), options{ "", "", "" }, correctIndex(0), x(0), y(0), solved(true)
{
}

Riddle::Riddle(const std::string& q,
    const std::string(&opts)[3],
    int correctIdx,
    int posX, int posY)
    : question(q), correctIndex(correctIdx), x(posX), y(posY), solved(false)
{
    options[0] = opts[0];
    options[1] = opts[1];
    options[2] = opts[2];
}