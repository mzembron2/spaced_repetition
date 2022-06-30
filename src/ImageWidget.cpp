#include "ImageWidget.h"

const float SCALE_MAX = 8.00;
const float SCALE_MIN = 0.03;

ImageWidget::ImageWidget(QWidget *parent, bool read_only_mode)
    : QWidget(parent)
    , ui_(new Ui::ImageWidget)
{
   ui_->setupUi(this);
   ui_ ->scroll_area->takeWidget();
   ui_ ->scroll_area->setWidget(ui_->image_label);
   ui_ -> scroll_area -> setVisible(true);

   if(read_only_mode)
   {
       ui_->open_button -> setVisible(false);
   }
   connect(ui_->open_button, SIGNAL(clicked()), this, SLOT(open()));
   connect(ui_->zoom_in_button, SIGNAL(clicked()), this, SLOT(zoomIn()));
   connect(ui_->zoom_out_button, SIGNAL(clicked()), this, SLOT(zoomOut()));

   QMenuBar *menu_bar = new QMenuBar();
   QMenu *view_menu = menu_bar->addMenu(tr("&View"));
   fit_to_window_act = view_menu->addAction(tr("&Fit to Window"), this, &::ImageWidget::fitToWindow);
   fit_to_window_act->setEnabled(false);
   fit_to_window_act->setCheckable(true);
   fit_to_window_act->setShortcut(tr("Ctrl+F"));
}

ImageWidget::ImageWidget(const ImageWidget& image_widget)
    : QWidget(image_widget.parentWidget())
    , ui_(new Ui::ImageWidget)
{

}

ImageWidget::~ImageWidget()
{
    delete ui_;
}

bool ImageWidget::loadFile(const QString &file_name)
{
    QImageReader reader(file_name);
    reader.setAutoTransform(true);
    const QImage new_image = reader.read();
    if (new_image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Nie można załadować %1: %2")
                                 .arg(QDir::toNativeSeparators(file_name), reader.errorString()));
        return false;
    }
    setImage(new_image);
    setWindowFilePath(file_name);
    makeImageName(file_name);
    return true;
}

QString ImageWidget::makeImageName(const QString &fil_path)
{
    std::string str = fil_path.toStdString();
    str = str.substr(str.find_last_of("/\\") + 1);
    image_name_ = QString::fromUtf8(str.c_str());
    return image_name_;
}

void ImageWidget::show(const QString &output_file, const QString deck_id)
{
    const QFileInfo fileInfo(__FILE__);
    const QDir main_dir = fileInfo.dir();
    const QString absolute_path = main_dir.absoluteFilePath("../Data/"+deck_id+"/"+output_file);
    loadFile(absolute_path);
}


QString ImageWidget::getName() const
{
    return image_name_;
}

bool ImageWidget::isAdded() const
{
    return !image_.isNull();
}

bool ImageWidget::saveFile(const QString &file_name)
{
    QImageWriter writer(file_name);

    if (!writer.write(image_)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Nie można zapisać %1: %2")
                                 .arg(QDir::toNativeSeparators(file_name)), writer.errorString());
        return false;
    }

    return true;
}

void ImageWidget::setNull()
{
    image_ = QImage();
    setImage(image_);
}

Ui::ImageWidget* ImageWidget::getUi()
{
    return ui_;
}

void ImageWidget::setImage(const QImage &new_image)
{
    image_ = new_image;
    if (image_.colorSpace().isValid())
        image_.convertToColorSpace(QColorSpace::SRgb);
    ui_ ->image_label->setPixmap(QPixmap::fromImage(image_));
    scale_factor_ = 1.0;

    ui_ -> scroll_area -> setVisible(true);
    fit_to_window_act->setEnabled(true);
    fitToWindow();

    if (!fit_to_window_act->isChecked())
        ui_ ->image_label->adjustSize();
}

void ImageWidget::fitToWindow()
{
    bool fitToWindow = fit_to_window_act->isChecked();
    ui_ -> scroll_area-> setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
}


void ImageWidget::normalSize()
{
    const float SCALE = 1.0;
    ui_ -> image_label->adjustSize();
    scale_factor_ = SCALE;
}

void ImageWidget::scaleImage(const double &factor)
{
    scale_factor_ *= factor;
    ui_ ->image_label->resize(scale_factor_ * ui_ ->image_label->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(ui_ ->scroll_area ->horizontalScrollBar(), factor);
    adjustScrollBar(ui_ ->scroll_area -> verticalScrollBar(), factor);

}

void ImageWidget::adjustScrollBar(QScrollBar *scroll_bar, const double &factor)
{
    scroll_bar->setValue(int(factor * scroll_bar->value()
                            + ((factor - 1) * scroll_bar->pageStep()/2)));
}


static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode accept_mode)
{
    static bool first_dialog = true;

    if (first_dialog) {
        first_dialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mime_type_filters;
    const QByteArrayList supported_mime_types = accept_mode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mime_type_name : supported_mime_types)
        mime_type_filters.append(mime_type_name);
    mime_type_filters.sort();
    dialog.setMimeTypeFilters(mime_type_filters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (accept_mode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}


bool ImageWidget::saveToPackageFolder(const QString &uniq_image_name,const QString &deck_id_name)
{
    const QFileInfo fileInfo(__FILE__);
    const QDir main_dir = fileInfo.dir();
    const QString absolute_path = main_dir.absoluteFilePath("../Data/"+deck_id_name+"/"+uniq_image_name);
    return saveFile(absolute_path);
}

//private slots start
void ImageWidget::open()
{
    QFileDialog dialog(this, tr("Otwórz plik"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}

void ImageWidget::zoomIn()
{
    const float SCALE = 1.25;
    if (!image_.isNull())
    {
        scaleImage(SCALE);
        if  (scale_factor_ > SCALE_MAX)
            ui_ -> zoom_in_button -> setEnabled(false);
        if  (scale_factor_ > SCALE_MIN)
            ui_ -> zoom_out_button -> setEnabled(true);
    }

}

void ImageWidget::zoomOut()
{
    const float SCALE = 0.8;
    if (!image_.isNull())
    {
        scaleImage(SCALE);
        if  (scale_factor_ < SCALE_MAX)
            ui_ -> zoom_in_button -> setEnabled(true);
        if  (scale_factor_ < SCALE_MIN)
            ui_ -> zoom_out_button -> setEnabled(false);
    }
}

//private slots end



