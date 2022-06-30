#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImageReader>
#include <QMenuBar>
#include <QImageWriter>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QColorSpace>
#include <QScrollBar>
#include "FlashcardWidget.h"
#include "../UI/ui_imagewidget.h"

/// The class represents image on the flashcard
/// (the image could be a question or an answer).
/// It provides methods for: loading/saving the
/// image from/to file and displaying it on GUI.

namespace Ui {
    class ImageWidget;
}

class ImageWidget : public QWidget, public FlashcardWidget
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = nullptr, bool read_only_mode= false);
    ImageWidget(const ImageWidget&);
    virtual ~ImageWidget();

    bool loadFile(const QString&);
    bool saveFile(const QString&);
    bool saveToPackageFolder(const QString&, const QString &deck_id_name);
    void setNull();

    void show(const QString&, const QString);
    QString makeImageName(const QString&);
    QString getName() const;
    bool isAdded() const;
    Ui::ImageWidget *getUi();



private slots:
    void open();
    void zoomIn();
    void zoomOut();

private:
    void setImage(const QImage&);
    void fitToWindow();
    void scaleImage(const double&);
    void normalSize();
    void adjustScrollBar(QScrollBar*, const double&);
    float scale_factor_;
    QAction *fit_to_window_act;
    Ui::ImageWidget *ui_;
    QImage image_;
    QString image_name_;



};

#endif // IMAGEWIDGET_H
