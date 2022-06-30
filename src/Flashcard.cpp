#include "Flashcard.h"

Flashcard::Flashcard(const int &flashcard_id, const int &deck_id, const int &type_front,
                     const  int &type_back, const QString &front, const QString &back, float& e_factor,
                     QDate &repetition_date,int &last_repetition_interval) :
    flashcard_id_(flashcard_id), deck_id_(deck_id), type_front_(type_front), type_back_(type_back), front_(front),
    back_(back), e_factor_(e_factor), repetition_date_(repetition_date),
    last_repetition_interval_(last_repetition_interval)
{

}
Flashcard::Flashcard(const Flashcard& other_flashcard) :
    flashcard_id_(other_flashcard.getFlashcardId()), type_front_(other_flashcard.getTypeFront()),
    deck_id_(other_flashcard.getDeckId()), type_back_(other_flashcard.getTypeBack()), front_(other_flashcard.getFront()),
    back_(other_flashcard.getBack()), e_factor_(other_flashcard.getEFactor()), repetition_date_(other_flashcard.getRepetitionDate()),
    last_repetition_interval_(other_flashcard.getLastRepetitionInterval())
{

}

const int Flashcard::getFlashcardId() const
{
    return flashcard_id_;
}

const int Flashcard::getDeckId()  const
{
    return deck_id_;
}

const int Flashcard::getTypeFront() const
{
    return type_front_;
}

const int Flashcard::getTypeBack() const
{
    return type_back_;
}

const QString Flashcard::getFront() const
{
    return front_;
}

const QString Flashcard::getBack() const
{
    return back_;
}

float Flashcard::getEFactor() const
{
    return e_factor_;
}

void Flashcard::setEFactor(const float& EFactor)
{
    e_factor_ = EFactor;
}

QDate Flashcard::getRepetitionDate() const
{
    return repetition_date_;
}

void Flashcard::setRepetitionDate(const QDate repetition_date)
{
    repetition_date_ = repetition_date;
}

int Flashcard::getLastRepetitionInterval() const
{
    return last_repetition_interval_;
}
void Flashcard::setLastRepetitionInterval(const int last_repetition_interval)
{
    last_repetition_interval_ = last_repetition_interval;
}
