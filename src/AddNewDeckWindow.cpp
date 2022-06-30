#include "AddNewDeckWindow.h"


AddNewDeckWindow::AddNewDeckWindow(QWidget *parent, DatabaseController *data_base) :
    QDialog(parent)
    ,database_(data_base)
    ,ui_(new Ui::AddNewDeckWindow)
{
    ui_->setupUi(this);

    //ui -> ui
    connect(ui_->add_new_deck_button, SIGNAL(clicked()), this, SLOT(addNewDeck()));
    connect(ui_->back_button, SIGNAL(clicked()), this, SLOT(backToPrevWindow()));

    //ui -> MainWindow
    connect(ui_->add_new_deck_button, SIGNAL(clicked()), this, SLOT(sendRefreshDecksView()));
}

AddNewDeckWindow::~AddNewDeckWindow()
{
    delete ui_;
}

void AddNewDeckWindow::addNewDeck()
{

    QString deck_name= ui_ -> textEdit -> toPlainText();

    if(deck_name.isEmpty())
    {
        ui_ -> show_info_label -> setText("Nazwa nowej talii jest pusta!");
    }
    else
    {
        if(database_->checkIfTableCellExists(deck_name))
        {
            ui_ -> show_info_label -> setText("Nazwa talii jest już zajęta!");
        }
        else
        {
            QVariant deck_id = database_ -> addDeck(deck_name);
            const QFileInfo fileInfo(__FILE__);
            const QDir mainDir = fileInfo.dir();
            const QString absolute_path = mainDir.absoluteFilePath("../Data/"+ deck_id.toString());
            mainDir.mkdir(absolute_path);
            ui_ -> show_info_label -> setText("Akcja zakończona. Pomyślnie dodano talie: "+ deck_name);
        }
    }

}

void AddNewDeckWindow::sendRefreshDecksView()
{
    /// signal emiter to MainWindow
    emit refreshDeckViewSignal();
}

void AddNewDeckWindow::backToPrevWindow()
{
    this -> accept();
}


