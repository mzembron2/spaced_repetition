#ifndef FLASHCARDWIDGET_H
#define FLASHCARDWIDGET_H

#include <QString>

/// Virtual class responsible for visualisation of
/// flashcards. It is a base class for: ImageWidget,
/// LearnWidget and SoundWidget.

class FlashcardWidget
{
public:
    FlashcardWidget();
    FlashcardWidget(const FlashcardWidget&);
    ~FlashcardWidget()= default;

    virtual void show(const QString&, const QString) = 0;
    virtual QString getName() const = 0;
    virtual bool isAdded() const = 0;
    virtual void setNull() = 0;
    virtual bool saveToPackageFolder(const QString&, const QString&) = 0;

};

#endif // FLASHCARDWIDGET_H
