#ifndef ALGORITHM_H
#define ALGORITHM_H

#include<cmath>
#include<QDate>
#include "Flashcard.h"

/// Implementation of spaced repetition SM-2 algortihm.
/// The class is responsible for computing the method
/// parameters according to which questions for the
/// user are selected.
///
/// Resources:
/// < https://en.wikipedia.org/wiki/Spaced_repetition > 
/// < https://en.wikipedia.org/wiki/SuperMemo#Description_of_SM-2_algorithm >

class Algorithm
{
public:
    Algorithm();
    void updateFlashcardParams(Flashcard &flashcard, const int &response_quality);
private:
    bool isIntervalValid(const int& interval);
    QDate calculateNextRepetitionDate(const QDate last_repetition_date, const int Interval);
    float countInterval(const float &e_factor, const float &previous_interval, const int &response_quality);
    float countNewEFactor(const float &e_factor, const int &response_quality);

};

#endif // ALGORITHM_H
