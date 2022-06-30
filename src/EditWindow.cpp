#include "EditWindow.h"


EditWindow::EditWindow(QWidget *parent, DatabaseController *data_base, const QString deck_name) :
    QDialog(parent)
    ,data_base_(data_base)
    ,deck_name_(deck_name)
    ,ui_(new Ui::EditWindow)
{
    deck_id_ = data_base_->getDeckId(deck_name_);

    ui_->setupUi(this);

    fillEditComboBox();

    ui_ -> show_deck_name_label -> setText(deck_name_);

    //front flashcard
    front_text_widget_= new TextWidget(this);
    ui_ ->front_flashcard_add_widget -> addTab(front_text_widget_, "tekst");
    front_image_widget_ = new ImageWidget(this);
    ui_ -> front_flashcard_add_widget -> addTab(front_image_widget_, "obraz");
    front_sound_widget_ = new SoundWidget(this);
    ui_ -> front_flashcard_add_widget -> addTab(front_sound_widget_, "dźwięk");

    //back flashcard
    back_text_widget_= new TextWidget(this);
    ui_ ->back_flashcard_add_widget -> addTab(back_text_widget_, "tekst");
    back_image_widget_ = new ImageWidget(this);
    ui_ -> back_flashcard_add_widget -> addTab(back_image_widget_, "obraz");
    back_sound_widget_ = new SoundWidget(this);
    ui_ -> back_flashcard_add_widget -> addTab(back_sound_widget_, "dźwięk");

    widgets_vector_ .push_back(front_text_widget_);
    widgets_vector_ .push_back(front_image_widget_);
    widgets_vector_ .push_back(front_sound_widget_);
    widgets_vector_ .push_back(back_text_widget_);
    widgets_vector_ .push_back(back_image_widget_);
    widgets_vector_ .push_back(back_sound_widget_);

    // ui_ -> ui_
    connect(ui_->add_flashcard_button, SIGNAL(clicked()), this, SLOT(addNewFlashcard()));
    connect(ui_->edit_flashcard_button, SIGNAL(clicked()), this, SLOT(editFlashcard()));
    connect(ui_->delete_flashcard_button, SIGNAL(clicked()), this, SLOT(deleteFlashcard()));
    connect(ui_->show_button,SIGNAL(clicked()), this, SLOT(showEditView()));
}

EditWindow::~EditWindow()
{
    delete ui_;
}

//private slots start

void EditWindow::addNewFlashcard()
{
    fillFlashcardQString();

    if(front_flashcard_QString_.isEmpty() || back_flashcard_QString_.isEmpty())
    {
        setInfoTextColor(false);
        if(front_flashcard_QString_.isEmpty())
            ui_ -> display_info_label -> setText("Uzupełnij przód karty!");
        else if(back_flashcard_QString_.isEmpty())
            ui_ -> display_info_label -> setText("Uzupełnij tył karty!");
    }
    else if(front_flashcard_QString_==back_flashcard_QString_)
    {
        setInfoTextColor(false);
        ui_ -> display_info_label -> setText("Przód i tył karty nie mogą być takie same!");
    }
    else
    {
        if(data_base_->checkIfFlashcardExists(front_flashcard_QString_, deck_id_.toInt()))
        {
           setInfoTextColor(false);
           ui_ -> display_info_label -> setText("Fiszka już istnieje!");
        }
        else
        {
            const QVariant new_id= data_base_-> addOneFlashcard(front_flashcard_type_,back_flashcard_type_, front_flashcard_QString_, back_flashcard_QString_, deck_id_.toInt());
            fillEditComboBox();
            setInfoTextColor(true);
            if(front_flashcard_type_!=0) //becouse off flashcard must be unique
            {
                front_flashcard_QString_ = new_id.toString()+front_flashcard_QString_;
                data_base_ -> updateOneFlashcardFront(front_flashcard_type_, front_flashcard_QString_, new_id.toInt());
            }
            if(back_flashcard_type_!=0) //becouse off flashcard must be unique
            {
                back_flashcard_QString_=new_id.toString()+back_flashcard_QString_;
                data_base_ -> updateOneFlashcardBack(back_flashcard_type_, back_flashcard_QString_, new_id.toInt());
            }
            ui_ -> display_info_label -> setText("Akcja zakończona. Pomyślnie dodano fiszke");
            clearFlashcardQString(new_id);
        }
    }
}

void EditWindow::editFlashcard()
{
    fillFlashcardQString();

    if(front_flashcard_QString_.isEmpty() || back_flashcard_QString_.isEmpty())
    {
        setInfoTextColor(false);
        if(front_flashcard_QString_.isEmpty())
            ui_ -> display_info_label -> setText("Uzupełnij przód karty!");
        else if(back_flashcard_QString_.isEmpty())
            ui_ -> display_info_label -> setText("Uzupełnij tył karty!");
    }
    else if(front_flashcard_QString_==back_flashcard_QString_)
    {
        setInfoTextColor(false);
        ui_ -> display_info_label -> setText("Przód i tył karty nie mogą być takie same!");
    }
    else
    {
        const int flashcard_id= data_base_ ->getFlashcardId(deck_id_.toInt(), front_flashcard_QString_);
        if (flashcard_id != -1)
        {
            QVariant current_back_content = data_base_ -> getOneFlashcardContent(QString::number(flashcard_id)).at(3);

            if(front_flashcard_QString_==QString::number(flashcard_id)+back_flashcard_QString_)
            {
                setInfoTextColor(false);
                ui_ -> display_info_label -> setText("Przód i tył karty nie mogą być takie same!");
            }
            else if(back_flashcard_QString_==current_back_content.toString())
            {
                setInfoTextColor(false);
                ui_ -> display_info_label -> setText("Nie zmieniono zawartości tyłu fiszki!");
            }
            else
            {
                if(data_base_->checkIfFlashcardExists(front_flashcard_QString_, deck_id_.toInt()))
                {
                   if(back_flashcard_type_ != 0)
                   {
                    back_flashcard_QString_=QString::number(flashcard_id) + back_flashcard_QString_;
                   }
                   const QVariant new_id =data_base_ -> updateOneFlashcardBack(back_flashcard_type_, back_flashcard_QString_, flashcard_id);
                   setInfoTextColor(true);
                   ui_ -> display_info_label -> setText("Akcja zakończona. Pomyślnie edytowano fiszke!");
                   clearFlashcardQString(new_id, true);

                }
                else
                {
                    setInfoTextColor(false);
                    ui_ -> display_info_label -> setText("Edytowana fiszka nie istnieje!");

                }
            }
        }
        else
        {
            setInfoTextColor(false);
            ui_ -> display_info_label -> setText("Edytowana fiszka nie istnieje!");
        }
    }
}

void EditWindow::deleteFlashcardContent(const QString &flashcard_id)
{
    const QFileInfo fileInfo(__FILE__);
    const QDir mainDir = fileInfo.dir();
    const QString absolute_path = mainDir.absoluteFilePath("../Data/"+ deck_id_.toString());
    QDir dir(absolute_path);
    QVariantList one_flashcard = data_base_ -> getOneFlashcardContent(flashcard_id);
    if(one_flashcard.at(0).toInt()!=0) //becouse textWidget dont save file
        dir.remove(one_flashcard.at(2).toString()); //remove front content
    if(one_flashcard.at(1).toInt()!=0)
        dir.remove(one_flashcard.at(3).toString()); //remove back content
}

void EditWindow::deleteFlashcard()
{
    if (ui_->flashcards_view->count()!=0 )
    {
         for(auto widget:widgets_vector_)
         {
            widget->setNull();
         }

        const int currentIndex = ui_ -> flashcards_view -> currentIndex();
        const QString flashcard_id = ui_ -> flashcards_view -> currentText();

        deleteFlashcardContent(flashcard_id);

        data_base_ -> deleteOneFlashcard(flashcard_id);
        ui_ ->flashcards_view-> removeItem(currentIndex);

        setInfoTextColor(true);
        ui_ -> display_info_label -> setText( "Akcja zakończona. Udało się usunąc element "+ flashcard_id);
    }
    else
    {
        setInfoTextColor(false);
        ui_ -> display_info_label -> setText("Brakuje elementu do usunięcia");
    }

}


void EditWindow::showEditView()
{
    if (ui_->flashcards_view->count()!=0 )
    {
        QString flashcard_id = ui_->flashcards_view->currentText();
        QVariantList flashcard_values = data_base_ ->getOneFlashcardContent(flashcard_id);

        int front_type = flashcard_values[0].toInt();
        int back_type = flashcard_values[1].toInt();
        QString front_output = flashcard_values[2].toString();
        QString back_output = flashcard_values[3].toString();

        widgets_vector_[front_type] -> show(front_output, deck_id_.toString());
        ui_ -> front_flashcard_add_widget -> setCurrentIndex(front_type);
        widgets_vector_[back_type+NUM_OF_TYPES] -> show(back_output, deck_id_.toString());
        ui_ -> back_flashcard_add_widget -> setCurrentIndex(back_type);
    }
    else
    {
        setInfoTextColor(false);
        ui_ -> display_info_label -> setText("Brakuje elementu do wyświetlenia");
    }

}

void EditWindow::fillEditComboBox()
{
    QStringList table_name = data_base_ ->getFlashcardsId(deck_id_.toInt());
    fillComboBox(table_name);

}

void EditWindow::fillComboBox(const QStringList &table_name)
{
    ui_->flashcards_view-> clear();
    if (table_name.length() == 0)
    {
        setInfoTextColor(false);
        ui_ -> display_info_label -> setText("Brak fiszek w talii");
    }
    else
    {
        ui_->flashcards_view->addItems(table_name);
        ui_->flashcards_view->setEnabled(true);
        ui_->flashcards_view->setFocus();
    }
}

void EditWindow::fillFlashcardQString()
{
    front_flashcard_type_ = ui_ -> front_flashcard_add_widget->currentIndex();
    back_flashcard_type_ = ui_ -> back_flashcard_add_widget->currentIndex();

    if (widgets_vector_[front_flashcard_type_]->isAdded())
    {
        front_flashcard_QString_=widgets_vector_[front_flashcard_type_] -> getName();
    }
    if (widgets_vector_[back_flashcard_type_+NUM_OF_TYPES]->isAdded())
    {
        back_flashcard_QString_=widgets_vector_[back_flashcard_type_+NUM_OF_TYPES] -> getName();

    }
}

void EditWindow::clearFlashcardQStringError(const QVariant &new_id)
{
    QMessageBox::warning(this, tr("BŁĄD"),
                                   tr("Nie udało się zapisać poprawnie pliku"));
    //usuwanie flascard

    data_base_ -> deleteOneFlashcard(new_id.toString());
    ui_->display_info_label ->setText("");
    fillEditComboBox();
}

void EditWindow::clearFlashcardQString(const QVariant &new_id, const bool only_editing)
{
    if(!only_editing) //add mode
    {
        if(!widgets_vector_[front_flashcard_type_]-> saveToPackageFolder(front_flashcard_QString_, deck_id_.toString()))
        {
            clearFlashcardQStringError(new_id);
        }
    }


    if(!widgets_vector_[back_flashcard_type_+NUM_OF_TYPES]-> saveToPackageFolder(back_flashcard_QString_,deck_id_.toString()))
    {
        clearFlashcardQStringError(new_id);
    }

    for(auto widget:widgets_vector_)
    {
        widget-> setNull();
    }
}


void EditWindow::setInfoTextColor(const bool &positive_message)
{
    QPalette sample_palette;
    if(positive_message)
    {
        sample_palette.setColor(QPalette::WindowText, Qt::darkGreen);
    }
    else
    {
        sample_palette.setColor(QPalette::WindowText, Qt::red);
    }
    ui_->display_info_label->setPalette(sample_palette);
}
