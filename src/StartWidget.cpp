#include "../UI/ui_startwidget.h"
#include "StartWidget.h"
#include "EditWindow.h"

StartWidget::StartWidget(QWidget *parent, DatabaseController *database)
    : QWidget(parent),
      ui_(new Ui::StartWidget),
      database_(database)
{
    ui_ ->setupUi(this);
    connect(ui_->new_deck_button, SIGNAL(clicked()), this->parent(), SLOT(goToAddNewDeckWindow()));
    connect(ui_->learn_button, SIGNAL(clicked()), this->parent(), SLOT(goToLearnWindow()));
    connect(ui_->edit_button, SIGNAL(clicked()), this->parent(), SLOT(goToEditWindow()));
    connect(ui_->delete_button, SIGNAL(clicked()), this, SLOT(deleteDeck()));

}

StartWidget::~StartWidget()
{
    delete ui_;
}

void StartWidget::fillCellNames(const QStringList &table_name)
{
    ui_->decks_view->clear();
    if (table_name.length() == 0)
        QMessageBox::warning(this, "Ostrzeżenie",
                             "Brak talii",
                             QMessageBox::Ok);
    else
    {
        ui_->decks_view->addItems(table_name);
        ui_->decks_view->setEnabled(true);
        ui_->decks_view->setFocus();
    }
}
void StartWidget::deleteDeck()
{
    const int current_index = ui_ -> decks_view -> currentIndex();
    const QString current_name = getCurrentDeckName();
    const QString current_id= database_ -> getDeckId(current_name).toString();

    database_ -> deleteDeck(current_name);

    ui_ -> decks_view ->removeItem(current_index);

    const QFileInfo fileInfo(__FILE__);
    const QDir mainDir = fileInfo.dir();
    const QString absolute_path = mainDir.absoluteFilePath("../Data/"+ current_id);
    QDir dir(absolute_path);
    dir.removeRecursively();
}


QString StartWidget::getCurrentDeckName() const
{
    QString deck_name = ui_ ->decks_view -> currentText();
    return deck_name;
}




