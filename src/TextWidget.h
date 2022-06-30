#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <QWidget>
#include "FlashcardWidget.h"

/// The class represents text on the flashcard
/// (the text could be a question or an answer).
/// Its main purpouse is to proprely display and
/// allow editing text (when read only mode is
/// disabled).

namespace Ui {
    class TextWidget;
}

class TextWidget : public QWidget, public FlashcardWidget
{
    Q_OBJECT
public:
    TextWidget(QWidget *parent = nullptr, bool read_only_mode= false);
    TextWidget(const TextWidget&);
    virtual ~TextWidget();

    void show(const QString &, const QString);
    QString getName() const;
    bool isAdded() const;
    void setNull();
    bool saveToPackageFolder(const QString&, const QString &deck_id_name);

signals:

private slots:
    void zoomTextIn();
    void zoomTextOut();

private:
    Ui::TextWidget *ui_;
    void zoom(const int&);

    int font_size;
};

#endif // TEXTWIDGET_H
