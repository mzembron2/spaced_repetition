#include "MainWindow.h"


const int START_WIDGET_INDEX = 2;
const int LEARN_WIDGET_INDEX = 3;

MainWindow::MainWindow(QWidget *parent, DatabaseController *data_base)
    : QMainWindow(parent)
    , data_base_(data_base)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    addNewDeckWindow_ = new AddNewDeckWindow(this, data_base_);
    start_widget_ = new StartWidget(this, data_base_);
    learn_widget_ = new LearnWidget(this, data_base_);

    ui_ ->stackedWidget -> addWidget(start_widget_);
    ui_ ->stackedWidget -> addWidget(learn_widget_);
    ui_ ->stackedWidget ->setCurrentIndex(START_WIDGET_INDEX);
    // fill combobox with decks names
    refreshDeckViewReceiver();

    //addNewDeck -> ui_
    connect(addNewDeckWindow_, SIGNAL(refreshDeckViewSignal()), this, SLOT(refreshDeckViewReceiver()));

}

MainWindow::~MainWindow()
{
    delete ui_;
    delete addNewDeckWindow_;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
        learn_widget_ ->endCurrentSession();
}
void MainWindow::goToAddNewDeckWindow()
{
    addNewDeckWindow_ -> setModal(true);
    addNewDeckWindow_ -> exec();
}

void MainWindow::goToLearnWindow()
{
    learn_widget_ -> setCurrentDeckName(start_widget_ -> getCurrentDeckName());
    ui_ ->stackedWidget ->setCurrentIndex(LEARN_WIDGET_INDEX);

}

void MainWindow::goToEditWindow()
{

    EditWindow editWindow(this, data_base_, start_widget_ -> getCurrentDeckName());
    editWindow.setModal(true);
    editWindow.exec();

}


QString MainWindow::getCurrentDeckName_() const
{
    return start_widget_->getCurrentDeckName();
}

void MainWindow::refreshDeckViewReceiver()
{
    ///signal receiver from AddNewDeckWindow
    QStringList table_cell_list = data_base_ -> getCellTableList();
    start_widget_ ->fillCellNames(table_cell_list);
}

void MainWindow::backToStartMenu()
{
    learn_widget_ ->stopRunningMusic();
    ui_ ->stackedWidget ->setCurrentIndex(START_WIDGET_INDEX);
}



