#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <QColor>
#include <QMenuBar>
#include "DatabaseController.h"
#include "TextWidget.h"
#include "ImageWidget.h"
#include "SoundWidget.h"
#include "FlashcardWidget.h"
#include "../UI/ui_editwindow.h"

/// Class is responsible for editing the flashcards
/// in the selected deck. It provides methods for
/// deleting  or editing an existing flashcard and
/// adding a new one.

namespace Ui {
class EditWindow;
}

class EditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = nullptr,  DatabaseController *data_base= nullptr, const QString = NULL);
    ~EditWindow();

private slots:
    void addNewFlashcard();
    void editFlashcard();
    void deleteFlashcard();
    void showEditView();


private:
    void fillEditComboBox();
    void fillFlashcardQString();
    void clearFlashcardQString(const QVariant &new_id, const bool only_editing=false);
    void clearFlashcardQStringError(const QVariant &new_id);
    void fillComboBox(const QStringList&);
    void setInfoTextColor(const bool &positive_message);
    void deleteFlashcardContent(const QString &flashcard_id);

    Ui::EditWindow *ui_;

    DatabaseController *data_base_;
    TextWidget *front_text_widget_;
    TextWidget *back_text_widget_;
    ImageWidget *front_image_widget_;
    ImageWidget *back_image_widget_;
    SoundWidget *front_sound_widget_;
    SoundWidget *back_sound_widget_;
    std::vector<FlashcardWidget*> widgets_vector_;

    int front_flashcard_type_;
    int back_flashcard_type_;
    QString front_flashcard_QString_;
    QString back_flashcard_QString_;

    const QString deck_name_;
    QVariant deck_id_;

    const int NUM_OF_TYPES=3;


};

#endif // EDITWINDOW_H
