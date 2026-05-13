#pragma once
#include <string>

struct QuestionTemplate {
    std::string question;
    std::string options[3];
    int correctIndex;
};

int getQuestionsCount();


const QuestionTemplate& getQuestionTemplate(int index);
