#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include "DatabaseController.h"
#include "AddNewDeckWindow.h"
#include "LearnWidget.h"

/// The class responsible for start menu,
/// manages its buttons and ComboBox with
/// names of decks

namespace Ui {
    class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StartWidget(QWidget *parent = nullptr, DatabaseController *database = nullptr);
    virtual ~StartWidget();
    void fillCellNames(const QStringList&);
    QString getCurrentDeckName() const;

private slots:
    void deleteDeck();
private:
    Ui::StartWidget *ui_;
    int deck_id_;
    DatabaseController *database_;


};

#endif // STARTWIDGET_H
