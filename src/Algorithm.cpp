#include "Algorithm.h"

const double LOWER_E_FACTOR_LIMIT = 1.3;
const int LOWER_RESPONSE_QUALITY_LIMIT = 3;
const int FIRST_REPETITION_INTERVAL = 1;
const int SECOND_REPETITION_INTERVAL = 6;

Algorithm::Algorithm()
{

}

void Algorithm::updateFlashcardParams(Flashcard &flashcard, const int &response_quality)
{
    flashcard.setEFactor(countNewEFactor(flashcard.getEFactor(), response_quality));
    flashcard.setLastRepetitionInterval(countInterval(flashcard.getEFactor(),flashcard.getLastRepetitionInterval(), response_quality));
    flashcard.setRepetitionDate(calculateNextRepetitionDate(flashcard.getRepetitionDate(),flashcard.getLastRepetitionInterval()));

}

QDate Algorithm::calculateNextRepetitionDate(QDate last_repetition_date, int interval)
{

    if(last_repetition_date.isValid())
    {
        return last_repetition_date.addDays(interval);
    }
    else
    {
        return last_repetition_date.currentDate().addDays(interval);
    }
}

float Algorithm::countInterval(const float& e_factor,const float& previous_interval, const int &response_quality)
{
    if (!isIntervalValid(previous_interval)) //check might not be equal nan
    {
        return FIRST_REPETITION_INTERVAL;
    }
    else if (response_quality <  LOWER_RESPONSE_QUALITY_LIMIT)
    {
        return FIRST_REPETITION_INTERVAL;
    }
    else if (previous_interval == FIRST_REPETITION_INTERVAL)
    {
        return SECOND_REPETITION_INTERVAL;
    }
    else
    {
        return round(previous_interval*e_factor);
    }
}

float Algorithm::countNewEFactor(const float& e_factor,const int& response_quality)
{
    // Formula for E_Factor - main parameter of SM-2 method.
    float new_e_factor = (e_factor + (0.1-(5-response_quality)*(0.08+(5-response_quality)*0.02)));
    if(new_e_factor < LOWER_E_FACTOR_LIMIT)
    {
        return LOWER_E_FACTOR_LIMIT;
    }
    else
    {
        return new_e_factor;
    }

}

bool Algorithm::isIntervalValid(const int& interval)
{
    return interval >0;
}
