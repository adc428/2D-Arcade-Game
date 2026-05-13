#pragma once
#include <string>

class Riddle {
    std::string question;
    std::string options[3];
    int correctIndex;
    int x, y;       // Position on map
    bool solved;

public:
    Riddle();
    Riddle(const std::string& q,
        const std::string(&opts)[3],
        int correctIdx,
        int posX, int posY);

    bool checkAnswer(int chosenIndex) const {
        return chosenIndex == correctIndex;
    }

    const std::string& getQuestion() const { return question; }
    const std::string& getOption(int i) const { return options[i]; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool isSolved() const { return solved; }
    void markSolved() { solved = true; }
};