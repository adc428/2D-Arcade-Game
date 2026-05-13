#include "Questions.h"

// מאגר סטטי של חידות
static QuestionTemplate QUESTIONS[] = {
    {
        "What has keys but no locks?",
        { "Piano", "Door", "Computer" },
        0 // Piano
    },
    {
        "What has hands but can't clap?",
        { "Tree", "Clock", "River" },
        1 // Clock
    },
    {
        "What can travel around the world while staying in a corner?",
        { "A stamp", "A car", "A plane" },
        0 // A stamp
    },
    {
        "What has a head and a tail but no body?",
        { "A snake", "A coin", "A worm" },
        1 // A coin
    },
    {
        "What gets wetter the more it dries?",
        { "A towel", "A sponge", "Water" },
        0 // A towel
    }
};

int getQuestionsCount() {
    return sizeof(QUESTIONS) / sizeof(QUESTIONS[0]);
}

const QuestionTemplate& getQuestionTemplate(int index) {
    return QUESTIONS[index];
}
