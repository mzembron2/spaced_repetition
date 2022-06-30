#ifndef ADDNEWDECKWINDOW_H
#define ADDNEWDECKWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QtWidgets>
#include <QtSql>
#include "DatabaseController.h"
#include "../UI/ui_addnewdeckwindow.h"

/// Class AddNewDeckWindow is implementation
/// of application side window, responsible for
/// adding new deck of questions.


namespace Ui {
class AddNewDeckWindow;
}

class AddNewDeckWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewDeckWindow(QWidget *parent = nullptr,  DatabaseController *data_base= nullptr);
    ~AddNewDeckWindow();

private slots:
    void addNewDeck();
    void backToPrevWindow();
    void sendRefreshDecksView();

signals:

    void refreshDeckViewSignal();

private:
    Ui::AddNewDeckWindow *ui_;

    DatabaseController *database_;
};

#endif // ADDNEWDECKWINDOW_H
