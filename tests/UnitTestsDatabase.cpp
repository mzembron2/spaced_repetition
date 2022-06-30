#include <gtest/gtest.h>
#include <QDir>
#include <QtSql>
#include "../src/DatabaseController.h"


// Test open and close connection methods in DatabaseController
TEST(DbConnectionTest, DbAssertions) 
{

    DatabaseController db_controller(0);
    QFileInfo fileInfo(__FILE__);
    QDir mainDir = fileInfo.dir();
    QString absolute_path_to_database = mainDir.absoluteFilePath("../Database/ProbaDataBase.db");
    db_controller.connectToSQLiteDatabaseRequested(absolute_path_to_database);

    EXPECT_EQ(1,db_controller.checkIfConnected());

    db_controller.dissconnectFromDatabaseRequested();
    EXPECT_EQ(0,db_controller.checkIfConnected());
}

TEST(DbConnectionTest, DbFlashcardsAssertions) 
{
    const int id = 1;
    const int type_front = 0;
    const int type_back = 0;
    const QString front = "test front";
    const QString back = "test back";
    float e_factor = 2.5;
    QDate repetition_date(2022, 1, 17);
    int last_repetition_interval = 1;




    DatabaseController db_controller(0);
    QFileInfo fileInfo(__FILE__);
    QDir mainDir = fileInfo.dir();
    QString absolute_path_to_database = mainDir.absoluteFilePath("../Database/ProbaDataBase.db");
    db_controller.connectToSQLiteDatabaseRequested(absolute_path_to_database);
    const QString deck_name = "testowa_nazwa1";
    db_controller.addDeck(deck_name);
    const int deck_id = db_controller.getDeckId(deck_name).toInt();
    db_controller.addOneFlashcard(type_front, type_back, front, back, deck_id);

    //Testing if flashcard is present in database and getFlashcard method works properly 
    std::vector<Flashcard> test_flashcards_vector;
    db_controller.getFlashcards(deck_name, &test_flashcards_vector);
    EXPECT_EQ(1, test_flashcards_vector.size());

    //Testing if flashcard was deleted from database
    db_controller.deleteDeck(deck_name);
    test_flashcards_vector.clear();
    db_controller.getFlashcards(deck_name, &test_flashcards_vector);
    EXPECT_EQ(0, test_flashcards_vector.size());
    db_controller.dissconnectFromDatabaseRequested();

}


