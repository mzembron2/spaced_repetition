#include <gtest/gtest.h>
#include <QDir>
#include <QtSql>
#include <iostream>
#include "../src/Algorithm.h"

TEST(AlgorithmTest, DateAndIntervalCalculation) 
{
    const int id = 1;
    const int deck_id = 1;
    const int type_front = 0;
    const int type_back = 0;
    const QString front = "test front";
    const QString back = "test back";
    float e_factor = 2.5;
    QDate repetition_date(2022, 1, 17);
    int last_repetition_interval = 5;

    Flashcard test_flashcard(id, deck_id, type_front, type_back, front, back, e_factor, repetition_date,
    last_repetition_interval);

    Algorithm sm2_algorithm;


    const int manually_calculated_interval = 13;
    QDate expected_repetition_date(2022, 1, 30);
    const int example_response_quality  = 5; 

    sm2_algorithm.updateFlashcardParams(test_flashcard,example_response_quality);

    EXPECT_EQ(test_flashcard.getRepetitionDate(), expected_repetition_date);
    EXPECT_EQ(test_flashcard.getLastRepetitionInterval(), manually_calculated_interval);

}
