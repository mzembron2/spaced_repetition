#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QString>
#include <QtSql>
#include <vector>
#include "Flashcard.h"

/// Class DatabaseController is responisble for
/// managing connection to database and executing
/// SQL queries

class DatabaseController : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseController(QObject*);
    ~DatabaseController();

    //check section
    bool checkIfConnected() const;
    bool checkIfTableCellExists(const QString);
    bool checkIfFlashcardExists(const QString, const int);

    //add, delete section
    QVariant addDeck(const QString&);
    QVariant addOneFlashcard(const int&, const int&, const QString&, const QString&, const int&);
    QVariant deleteDeck(const QString&);
    QVariant deleteFlashcards(const int &deck_id);
    QVariant deleteOneFlashcard(const QString &flashcard_id);
    QVariant updateOneFlashcardBack(const int &back_type, const QString &back, const int &flashcard_id);
    QVariant updateOneFlashcardFront(const int &front_type, const QString &front, const int &flashcard_id);

    //get section
    QVariant getDeckId(const QString&);
    QString getQStringDeckId(const QString&);
    QStringList getCellTableList();
    QVariantList getOneFlashcardContent(const QString&);
    void getFlashcards(const QString& deck_name,std::vector<Flashcard> *current_session_flashcards);
    void updateFlashcardsParams(const std::vector<Flashcard> *current_session_flashcards);
    QStringList getFlashcardsId(const int&);
    int getFlashcardId( const int& deck_id, const QString& front);
    QSqlError getLastSQLError() const;

public slots:
    void connectToSQLiteDatabaseRequested(const QString);
    void dissconnectFromDatabaseRequested();

signals:
    void serverConnected();
    void serverErrorWithConnection(const QString);
    void serverDisconnected();

private:
    QSqlDatabase database_;

    bool connectToSQLiteDatabase(const QString);
    void disconnectFromDatabase();   

    QSqlQuery makeQSqlQuery(const QLatin1String &command);

};

#endif // DATABASECONTROLLER_H
