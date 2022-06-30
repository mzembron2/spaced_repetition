#include <gtest/gtest.h>
#include <QDir>
#include <QtSql>
#include <QDateTime>
#include <QString>
#include "../src/Flashcard.h"

TEST(FlashcardTest, FlashcardCreation) 
{
    const int id = 1;
    const int deck_id = 1;
    const int type_front = 0;
    const int type_back = 0;
    const QString front = "test front";
    const QString back = "test back";
    float e_factor = 2.5;
    QDate repetition_date(2022, 1, 17);
    int last_repetition_interval = 1;

    Flashcard test_flashcard(id, deck_id, type_front, type_back, front, back, e_factor, repetition_date,
    last_repetition_interval);

    EXPECT_EQ(test_flashcard.getFlashcardId(), id);
    EXPECT_EQ(test_flashcard.getDeckId(), deck_id);
    EXPECT_EQ(test_flashcard.getTypeFront(), type_front);
    EXPECT_EQ(test_flashcard.getTypeBack(), type_back);
    EXPECT_EQ(test_flashcard.getFront(), front);
    EXPECT_EQ(test_flashcard.getBack(), back);
    EXPECT_EQ(test_flashcard.getEFactor(), e_factor);
    EXPECT_EQ(test_flashcard.getRepetitionDate(), repetition_date);
    EXPECT_EQ(test_flashcard.getLastRepetitionInterval(), last_repetition_interval);

}


TEST(FlashcardTest, FlashcardCopyConstructor) 
{
    const int id = 1;
    const int deck_id = 1;
    const int type_front = 0;
    const int type_back = 0;
    const QString front = "test front";
    const QString back = "test back";
    float e_factor = 2.5;
    QDate repetition_date(2022, 1, 17);
    int last_repetition_interval = 1;

    Flashcard original_flashcard(id, deck_id, type_front, type_back, front, back, e_factor, repetition_date,
    last_repetition_interval);
    Flashcard copy_flashcard(original_flashcard);

    EXPECT_EQ(copy_flashcard.getFlashcardId(), id);
    EXPECT_EQ(copy_flashcard.getDeckId(), deck_id);
    EXPECT_EQ(copy_flashcard.getTypeFront(), type_front);
    EXPECT_EQ(copy_flashcard.getTypeBack(), type_back);
    EXPECT_EQ(copy_flashcard.getFront(), front);
    EXPECT_EQ(copy_flashcard.getBack(), back);
    EXPECT_EQ(copy_flashcard.getEFactor(), e_factor);
    EXPECT_EQ(copy_flashcard.getRepetitionDate(), repetition_date);
    EXPECT_EQ(copy_flashcard.getLastRepetitionInterval(), last_repetition_interval);

}


