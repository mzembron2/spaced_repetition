#ifndef FLASHCARD_H
#define FLASHCARD_H
#include <QString>
#include <QDateTime>

/// The class represesnts the flashcard object.
/// Contains its most important parameters.

class Flashcard
{
public:
    Flashcard(const int &flashcard_id, const int &deck_id, const int &type_front,
                       const int &type_back, const QString &front, const QString &back,
                       float &e_factor, QDate &repetition_date,
                       int &repetition_interval);
    Flashcard(const Flashcard& other_fashcard);
    Flashcard& operator= (Flashcard& other_flashcard) = delete;
    const int getFlashcardId() const;
    const int getDeckId() const;
    const int getTypeFront() const;
    const int getTypeBack() const;
    const QString getFront() const;
    const QString getBack() const;
    float getEFactor() const ;
    void setEFactor(const float& EFactor);
    QDate getRepetitionDate() const;
    void setRepetitionDate(const QDate repetition_date);
    int getLastRepetitionInterval() const;
    void setLastRepetitionInterval(const int last_repetition_interval);
private:
    const int flashcard_id_;
    const int deck_id_;
    const int type_front_;
    const int type_back_;
    const QString front_;
    const QString back_;
    float e_factor_;
    QDate repetition_date_;
    int last_repetition_interval_;

};

#endif // FLASHCARD_H
