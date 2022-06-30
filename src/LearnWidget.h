#ifndef LEARNWIDGET_H
#define LEARNWIDGET_H

#include <QWidget>
#include <vector>
#include <QSignalMapper>
#include <QShortcut>
#include <QSignalMapper>
#include "Flashcard.h"
#include "DatabaseController.h"
#include "FlashcardWidget.h"
#include "SoundWidget.h"
#include "ImageWidget.h"
#include "TextWidget.h"
#include "Algorithm.h"
#include "qstackedwidget.h"
#include "../UI/ui_learnwidget.h"

/// The class is responsible for learning section
/// of application. Its main purpose is to "guide"
/// user through the questions, by displaying
/// them on GUI and managing the answers.
/// To achieve this goal, it communicates with
/// the database (by pointer to its object) and
/// uses Algorithm class - to calculate
/// next repetition date.

namespace Ui {
    class LearnWidget;
}

class LearnWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LearnWidget(QWidget *parent = nullptr, DatabaseController *database = nullptr);
    virtual ~LearnWidget();
    void setCurrentDeckName(QString deck_name);
    void endCurrentSession();
    void stopRunningMusic();

private slots:
    void beginSession();
    void showAnswer();
    void refreshQuestions();
    void manageAnswer(const int quality);

private:
    Ui::LearnWidget *ui_;
    SoundWidget *sound_question_widget_;
    ImageWidget *image_question_widget_;
    TextWidget *text_question_widget_;
    SoundWidget *sound_answer_widget_;
    ImageWidget *image_answer_widget_;
    TextWidget *text_answer_widget_;
    DatabaseController *database_;
    std::vector<Flashcard> current_session_flashcards_;
    Algorithm sm2_algorithm_;
    QString current_deck_name_;
    int current_question_index_;
    void displayBeginLayout();
    void displayQuestionLayout();
    void displayAnswerLayout();
    void displayEndPageLayout();
    void goToNextQuestion();
    void prepareNextQuestion();
    void manageLackingQuestions();
    void updateFlashcardParams(const int &response_quality);
    void connectButtons();
    void connectWidgets();
    void displayProperQuestionWidget(const Flashcard &flascard);
    void displayProperAnswerWidget(const Flashcard &flashcard);
    void getCurrentSessionFlashcards();
    void sendFlashcardsParamsToDatabase();

};

#endif // LEARNWIDGET_H
