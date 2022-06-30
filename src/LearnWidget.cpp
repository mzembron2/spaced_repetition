#include "LearnWidget.h"


const int START_INDEX = 0;
//const int QUESTION_INDEX = 1;

enum Widget_index{
    TEXT = 0,
    IMAGE = 1,
    SOUND = 2
};

LearnWidget::LearnWidget(QWidget *parent, DatabaseController *database)
    : QWidget(parent),
      database_(database),
      ui_(new Ui::LearnWidget)
{
    ui_->setupUi(this);
    displayBeginLayout();
    connectWidgets();
    connectButtons();
    current_question_index_ = START_INDEX;
}

LearnWidget::~LearnWidget()
{
    delete ui_;
}

void LearnWidget::setCurrentDeckName(QString deck_name)
{
    const QString EMPTY_SESSION_NAME = "";
    if(current_deck_name_ != deck_name)
    {
        if(current_deck_name_ != EMPTY_SESSION_NAME)
        {
            endCurrentSession();
            displayBeginLayout();
        }
        current_deck_name_ = deck_name;
    }
}

void LearnWidget::beginSession()
{
    goToNextQuestion();
}

void LearnWidget::showAnswer()
{
    displayAnswerLayout();
}

void LearnWidget::refreshQuestions()
{
    manageLackingQuestions();
}


void LearnWidget::manageAnswer(const int quality)
{
    updateFlashcardParams(quality);
    goToNextQuestion();
}


void LearnWidget::prepareNextQuestion()
{
    const int MAXIMUM_QUESTION_INDEX = current_session_flashcards_.size() - 1;
    if(current_question_index_ < MAXIMUM_QUESTION_INDEX)
    {
        ++current_question_index_;
        displayQuestionLayout();
    }
    else
    {
        endCurrentSession();
    }
}

void LearnWidget::displayBeginLayout()
{
    ui_ ->stacked_question_widget -> setCurrentIndex(START_INDEX);
    ui_ ->stacked_answer_widget -> setCurrentIndex(START_INDEX);
    ui_ ->stacked_main_widgets ->setCurrentWidget(ui_ -> start_session_page);
}

void LearnWidget::goToNextQuestion()
{
     if (current_session_flashcards_.empty())
     {
         manageLackingQuestions();
     }
     else
     {
        prepareNextQuestion();
     }
}

void  LearnWidget::manageLackingQuestions()
{
    getCurrentSessionFlashcards();
    if (current_session_flashcards_.empty())
    {
        displayEndPageLayout();
    }
    else
    {
        displayQuestionLayout();
    }
}


void LearnWidget::displayQuestionLayout()
{
     stopRunningMusic();
     ui_ ->stacked_main_widgets ->setCurrentWidget(ui_ -> question_page);
     displayProperQuestionWidget(
                     current_session_flashcards_[current_question_index_]
                     );

}

void LearnWidget::displayEndPageLayout()
{
     stopRunningMusic();
     ui_ ->stacked_main_widgets ->setCurrentWidget(ui_ -> end_page);
}

void LearnWidget::displayAnswerLayout()
{
    stopRunningMusic();
    ui_ ->stacked_main_widgets ->setCurrentWidget(ui_ -> answer_page);
    displayProperAnswerWidget(
                    current_session_flashcards_[current_question_index_]
                    );

}

void LearnWidget::endCurrentSession()
{
    sendFlashcardsParamsToDatabase();
    current_session_flashcards_.clear();
    current_question_index_ = START_INDEX;
    displayEndPageLayout();
}

void LearnWidget::updateFlashcardParams(const int &response_quality)
{
    sm2_algorithm_.updateFlashcardParams(current_session_flashcards_[current_question_index_], response_quality);
    qDebug() << "Flashcard new efactor: " << current_session_flashcards_[current_question_index_].getEFactor();
    qDebug() << "Flashcard new next repetition date: " << current_session_flashcards_[current_question_index_].getRepetitionDate();
    qDebug() << "Flashcard new interval: " << current_session_flashcards_[current_question_index_].getLastRepetitionInterval() ;
}

void LearnWidget::connectButtons()
{
    connect(ui_->go_to_start_button, SIGNAL(clicked()), this->parent(), SLOT(backToStartMenu()));
    connect(ui_-> start_session_button, SIGNAL(clicked()), this, SLOT(beginSession()));
    connect(ui_ ->show_answer_button, SIGNAL(clicked()), this, SLOT(showAnswer()));
    connect(ui_ -> refresh_button, SIGNAL(clicked()), this, SLOT(refreshQuestions()));

    QSignalMapper* signal_mapper = new QSignalMapper (this) ;
    connect(ui_ ->one_button, SIGNAL(clicked()), signal_mapper, SLOT(map())) ;
    connect(ui_ ->two_button, SIGNAL(clicked()),  signal_mapper, SLOT(map()));
    connect(ui_ ->three_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    connect(ui_ ->four_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
    connect(ui_ ->five_button, SIGNAL(clicked()),  signal_mapper, SLOT(map()));
    signal_mapper -> setMapping (ui_ ->one_button, 1);
    signal_mapper -> setMapping (ui_ ->two_button, 2);
    signal_mapper -> setMapping (ui_ ->three_button, 3);
    signal_mapper -> setMapping (ui_ ->four_button, 4);
    signal_mapper -> setMapping (ui_ ->five_button, 5);
    connect (signal_mapper, SIGNAL(mapped(int)), this, SLOT(manageAnswer(int))) ;
}

void LearnWidget::connectWidgets()
{
    image_question_widget_ = new ImageWidget(this, true);
    text_question_widget_ = new TextWidget(this, true);
    sound_question_widget_ = new SoundWidget(this, true);
    image_answer_widget_ = new ImageWidget(this, true);
    text_answer_widget_ = new TextWidget(this, true);
    sound_answer_widget_ = new SoundWidget(this, true);
    ui_ ->stacked_question_widget ->addWidget(image_question_widget_);
    ui_ ->stacked_question_widget ->addWidget(text_question_widget_);
    ui_ ->stacked_question_widget -> addWidget(sound_question_widget_);
    ui_ ->stacked_answer_widget ->addWidget(image_answer_widget_);
    ui_ ->stacked_answer_widget ->addWidget(text_answer_widget_);
    ui_ ->stacked_answer_widget -> addWidget(sound_answer_widget_);
}

void LearnWidget::displayProperQuestionWidget(const Flashcard &flashcard)
{
    const int type = flashcard.getTypeFront();
    const QString content = flashcard.getFront();
    const QString deck_id_str = QString::number(flashcard.getDeckId());
//    const QString flashcard_id_str = QString::number(flashcard.getFlashcardId());
    if (type == Widget_index(TEXT))
    {
        ui_ ->stacked_question_widget ->setCurrentWidget(text_question_widget_);
        text_question_widget_ -> show(content, deck_id_str);
    }
    else if (type == Widget_index(IMAGE))
    {
        ui_ ->stacked_question_widget ->setCurrentWidget(image_question_widget_);
        image_question_widget_ -> show(content, deck_id_str);
    }
    else if (type == Widget_index(SOUND))
    {
        ui_ ->stacked_question_widget->setCurrentWidget(sound_question_widget_);
        sound_question_widget_ -> show(content, deck_id_str);
    }
}

void LearnWidget::displayProperAnswerWidget(const Flashcard &flashcard)
{
    const int type = flashcard.getTypeBack();
    const QString content = flashcard.getBack();
    const QString deck_id_str = QString::number(flashcard.getDeckId());
//    const QString flashcard_id_str = QString::number(flashcard.getFlashcardId());
    if (type == Widget_index(TEXT))
    {
        ui_ ->stacked_answer_widget ->setCurrentWidget(text_answer_widget_);
        text_answer_widget_ -> show(content, deck_id_str);
    }
    else if (type == Widget_index(IMAGE))
    {
        ui_ ->stacked_answer_widget ->setCurrentWidget(image_answer_widget_);
        image_answer_widget_ -> show(content, deck_id_str);
    }
    else if (type == Widget_index(SOUND))
    {
        ui_ ->stacked_answer_widget->setCurrentWidget(sound_answer_widget_);
        sound_answer_widget_ -> show(content, deck_id_str);
    }
}


void LearnWidget::getCurrentSessionFlashcards()
{
    database_ ->getFlashcards(current_deck_name_, &current_session_flashcards_);
}

void LearnWidget::sendFlashcardsParamsToDatabase()
{
    database_ ->updateFlashcardsParams(&current_session_flashcards_);
}

void LearnWidget::stopRunningMusic()
{
    sound_answer_widget_ ->stop();
    sound_question_widget_ -> stop();
}



