#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>
#include <QCloseEvent>
#include "DatabaseController.h"
#include "AddNewDeckWindow.h"
#include "LearnWidget.h"
#include "EditWindow.h"
#include "StartWidget.h"
#include "../UI/ui_mainwindow.h"

/// Class MainWindow is responisble for interface of
/// application main window. It aggregates any other
/// window and manages switching between them.


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, DatabaseController *data_base= nullptr);
    ~MainWindow();
private slots:
    void goToAddNewDeckWindow();
    void goToLearnWindow();
    void goToEditWindow();
    void refreshDeckViewReceiver();
    void backToStartMenu();

private:
    Ui::MainWindow *ui_;
    QSqlRelationalTableModel *model_;
    DatabaseController *data_base_;
    AddNewDeckWindow *addNewDeckWindow_;
    QGraphicsScene *scene_proba_;
    QGraphicsView *view_proba_;
    LearnWidget *learn_widget_;
    StartWidget *start_widget_;
    QString getCurrentDeckName_() const;
    void fillCellNames(const QStringList&);
    void closeEvent (QCloseEvent *event);
};
#endif // MAINWINDOW_H
