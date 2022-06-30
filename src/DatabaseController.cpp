#include "DatabaseController.h"

const QLatin1String SELECT_DECK_SQL = QLatin1String(R"(
SELECT name FROM Deck
    )");

const QLatin1String INSERT_DECK_SQL = QLatin1String(R"(
    INSERT INTO Deck (name)
                      VALUES(?)
    )");

const QLatin1String INSERT_FLASHCARD_SQL = QLatin1String(R"(
    INSERT INTO Flashcard (deck_id, type_front, type_back, front, back, e_factor)
                      VALUES(?, ?, ?, ?, ?, ?)
    )");
const QLatin1String UPDATE_FLASHCARD_PARAMS_SQL = QLatin1String(R"(
    UPDATE Flashcard SET e_factor = (?), repetition_date = (?), last_repetition_interval = (?)
    WHERE flashcard_id = (?)
    )");

const QLatin1String UPDATE_FLASHCARD_BACK_SQL = QLatin1String(R"(
    UPDATE Flashcard SET type_back = (?), back = (?)
    WHERE flashcard_id = (?)
    )");

const QLatin1String UPDATE_FLASHCARD_FRONT_SQL = QLatin1String(R"(
    UPDATE Flashcard SET type_front = (?), front = (?)
    WHERE flashcard_id = (?)
    )");


const QLatin1String DELETE_DECK_SQL = QLatin1String(R"(
    DELETE FROM Deck WHERE name=(?)
    )");

const QLatin1String SELECT_DECK_ID_SQL = QLatin1String(R"(
    SELECT deck_id FROM Deck WHERE name=(?)
    )");

const QLatin1String SELECT_FLASHCARD_ID_SQL = QLatin1String(R"(
    SELECT flashcard_id FROM Flashcard WHERE deck_id=(?) AND front=(?)
    )");

const QLatin1String SELECT_FLASHCARDS_SQL = QLatin1String(R"(
    SELECT f.flashcard_id, f.deck_id, f.type_front, f.type_back, f.front, f.back, f.e_factor,
    f.repetition_date, f.last_repetition_interval FROM Flashcard f left join Deck d on (f.deck_id = d.deck_id) WHERE d.name=(?)
    AND (f.repetition_date = (?) OR f.repetition_date IS NULL)
    )");

const QLatin1String SELECT_FLASHCARDS_FRONT_SQL = QLatin1String(R"(
    SELECT front FROM Flashcard WHERE deck_id=(?)
    )");
const QLatin1String SELECT_ONE_FLASHCARD = QLatin1String(R"(
    SELECT type_front, type_back, front, back FROM Flashcard WHERE flashcard_id=(?)
    )");

const QLatin1String SELECT_FLASHCARDS_ID_SQL = QLatin1String(R"(
    SELECT flashcard_id FROM Flashcard WHERE deck_id=(?)
    )");
const QLatin1String DELETE_FLASHCARD_BY_DECK_ID_SQL = QLatin1String(R"(
    DELETE FROM Flashcard WHERE deck_id=(?)
    )");

const QLatin1String DELETE_FLASHCARD_BY_FLASHCARD_ID = QLatin1String(R"(
    DELETE FROM Flashcard WHERE flashcard_id=(?)
    )");

DatabaseController::DatabaseController(QObject* parent) :
    QObject(parent)
{

}

DatabaseController::~DatabaseController()
{
    if (database_.isOpen())
        database_.close();
}

// connect section start
void DatabaseController::connectToSQLiteDatabaseRequested(const QString path_to_database)
{
    database_ = QSqlDatabase();
    database_.removeDatabase("supermemo-db"); //remove possible old connection
    database_ = QSqlDatabase::addDatabase("QSQLITE", "supermemo-db");
    bool connection_succesfull;
    connection_succesfull = connectToSQLiteDatabase(path_to_database);

    if (connection_succesfull)
        emit serverConnected();
    else
        emit serverErrorWithConnection(getLastSQLError().driverText());
}

void DatabaseController::dissconnectFromDatabaseRequested()
{
    disconnectFromDatabase();
    emit serverDisconnected();
}

bool DatabaseController::connectToSQLiteDatabase(const QString path_to_database)
{
    database_.setDatabaseName(path_to_database);
    return database_.open();
}

void DatabaseController::disconnectFromDatabase(){
    database_.close();
}
// connect section end

//make section start
QSqlQuery DatabaseController::makeQSqlQuery(const QLatin1String &command)
{
    QSqlQuery q(database_);
    if (!q.prepare(command))
            qDebug()<< q.lastError();
    return q;
}
//make section end

//check function start
bool DatabaseController::checkIfConnected() const
{
    return database_.isOpen();
}

bool DatabaseController::checkIfTableCellExists(const QString cell_name)
{
    //not const function becouse of getCellTableList
    return getCellTableList().contains(cell_name);
}


bool DatabaseController::checkIfFlashcardExists(const QString cell_name, const int deck_id)
{
    QSqlQuery q= makeQSqlQuery(SELECT_FLASHCARDS_FRONT_SQL);
    q.addBindValue(deck_id);
    q.exec();
    while(q.next())
    {
        if(q.value(0).toString()== cell_name)
            return true;
    }
    return false;
}

//check function end


//add delete section start
QVariant DatabaseController::addDeck(const QString &name)
{
    QSqlQuery q = makeQSqlQuery(INSERT_DECK_SQL);
    q.addBindValue(name);
    q.exec();
    return q.lastInsertId();

}

QVariant DatabaseController::deleteOneFlashcard(const QString &flashcard_id)
{
    QSqlQuery q = makeQSqlQuery(DELETE_FLASHCARD_BY_FLASHCARD_ID);
    q.addBindValue(flashcard_id);
    q.exec();
    return q.lastInsertId();
}

QVariant DatabaseController::addOneFlashcard(const int &front_type, const int &back_type, const QString &front_fc, const QString &back_fc, const int &deck_id)
{
    const float e_factor_value= 2.5;

    QSqlQuery q = makeQSqlQuery(INSERT_FLASHCARD_SQL);
    q.addBindValue(deck_id);
    q.addBindValue(front_type);
    q.addBindValue(back_type);
    q.addBindValue(front_fc);
    q.addBindValue(back_fc);
    q.addBindValue(e_factor_value);
    q.exec();

    return q.lastInsertId();
}
QVariant DatabaseController::deleteDeck(const QString &name)
{
    int deck_id = getDeckId(name).toInt();
    deleteFlashcards(deck_id);

    QSqlQuery q= makeQSqlQuery(DELETE_DECK_SQL);
    q.addBindValue(name);
    q.exec();
    return q.lastInsertId();

}

QVariant DatabaseController::deleteFlashcards(const int &deck_id)
{
    //function delete all flashcards from flashcard base with given deck_id
    QSqlQuery q = makeQSqlQuery(DELETE_FLASHCARD_BY_DECK_ID_SQL);
    q.addBindValue(deck_id);
    q.exec();
    return q.lastInsertId();
}
//add delete section end

//get section start
QSqlError DatabaseController::getLastSQLError() const
{
    return database_.lastError();
}

QVariantList DatabaseController::getOneFlashcardContent(const QString& flashcard_id)
{
    QVariantList flashcard_values;
    QSqlQuery querry= makeQSqlQuery(SELECT_ONE_FLASHCARD);
    querry.addBindValue(flashcard_id);
    querry.exec();
    while (querry.next()) {
        flashcard_values.push_back(querry.value(0));
        flashcard_values.push_back(querry.value(1));
        flashcard_values.push_back(querry.value(2));
        flashcard_values.push_back(querry.value(3));
    }
    return flashcard_values;
}


void DatabaseController::getFlashcards(const QString& deck_name, std::vector<Flashcard> *current_session_flashcards)
{
    const QDate TODAYS_DATE;
    QSqlQuery querry= makeQSqlQuery(SELECT_FLASHCARDS_SQL);
    querry.addBindValue(deck_name);
    querry.addBindValue( TODAYS_DATE.currentDate());
    querry.exec();
    while (querry.next()) {
        int flashcard_id = querry.value(0).toInt();
        int deck_id = querry.value(1).toInt();
        int type_front = querry.value(2).toInt();
        int type_back = querry.value(3).toInt();
        QString front  = querry.value(4).toString();
        QString back = querry.value(5).toString();
        float EFactor = querry.value(6).toFloat();
        QDate repetition_date = querry.value(7).toDate();
        int last_repetition_interval = querry.value(8).toInt();
        Flashcard flashcard(flashcard_id, deck_id, type_front, type_back, front, back, EFactor, repetition_date, last_repetition_interval);
        current_session_flashcards ->push_back(flashcard);
    }
}

void DatabaseController::updateFlashcardsParams(const std::vector<Flashcard> *current_session_flashcards)
{
    QSqlQuery querry= makeQSqlQuery(UPDATE_FLASHCARD_PARAMS_SQL);
    QVariantList e_factors, repetition_dates, repetition_intervals, flashcard_ids;

    for (auto &flashcard : *current_session_flashcards)
    {
        e_factors << flashcard.getEFactor();
        repetition_dates << flashcard.getRepetitionDate();
        repetition_intervals << flashcard.getLastRepetitionInterval();
        flashcard_ids << flashcard.getFlashcardId();
    }

    querry.addBindValue(e_factors);
    querry.addBindValue(repetition_dates);
    querry.addBindValue(repetition_intervals);
    querry.addBindValue(flashcard_ids);
    if (!querry.execBatch())
        qDebug() << querry.lastError();
}


QVariant DatabaseController::getDeckId(const QString &name)
{
    QSqlQuery q = makeQSqlQuery(SELECT_DECK_ID_SQL);
    q.addBindValue(name);
    q.exec();
    while (q.next()) {
        return q.value(0);
    }
    return 0;
}
QString DatabaseController::getQStringDeckId(const QString &name)
{
    QSqlQuery q = makeQSqlQuery(SELECT_DECK_ID_SQL);
    q.addBindValue(name);
    q.exec();
    while (q.next()) {
        return q.value(0).toString();
    }
    return "";
}
QStringList DatabaseController::getFlashcardsId(const int& deck_id)
{
    QStringList flashcards_id_vec;
    QSqlQuery q= makeQSqlQuery(SELECT_FLASHCARDS_ID_SQL);
    q.addBindValue(deck_id);
    q.exec();
    while (q.next()) {
        flashcards_id_vec.push_back(q.value(0).toString());
    }
    return flashcards_id_vec;
}

int DatabaseController::getFlashcardId(const int& deck_id, const QString& front)
{
    QSqlQuery q= makeQSqlQuery(SELECT_FLASHCARD_ID_SQL);
    q.addBindValue(deck_id);
    q.addBindValue(front);
    q.exec();
    while (q.next()) {
        return q.value(0).toInt();
    }
    return -1;
}


QStringList DatabaseController::getCellTableList()
{
    QStringList cell_table_list;
    QSqlQuery q= makeQSqlQuery(SELECT_DECK_SQL);
    q.exec();

    while (q.next()) {
        QString n = q.value(0).toString();
        cell_table_list.append(n);
    }
    return cell_table_list;
}
//get section end

//update
QVariant DatabaseController::updateOneFlashcardBack(const int& back_type, const QString& back, const int& flashcard_id)
{
    QSqlQuery q= makeQSqlQuery(UPDATE_FLASHCARD_BACK_SQL);
    q.addBindValue(back_type);
    q.addBindValue(back);
    q.addBindValue(flashcard_id);
    q.exec();
    return q.lastInsertId();
}

QVariant DatabaseController::updateOneFlashcardFront(const int &front_type, const QString &front, const int &flashcard_id)
{
    QSqlQuery q= makeQSqlQuery(UPDATE_FLASHCARD_FRONT_SQL);
    q.addBindValue(front_type);
    q.addBindValue(front);
    q.addBindValue(flashcard_id);
    q.exec();
    return q.lastInsertId();
}





