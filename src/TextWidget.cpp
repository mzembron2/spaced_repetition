#include "TextWidget.h"
#include "../UI/ui_textwidget.h"

const int FONT_SIZE_MIN = 5;
const int FONT_SIZE_MAX = 50;
//FONT_SIZE_MIN-FONT_SIZE_STEP must be greater that 0
const int FONT_SIZE_STEP = 2;

TextWidget::TextWidget(QWidget *parent, bool read_only_mode)
    : QWidget(parent)
    , ui_(new Ui::TextWidget)
{
    ui_->setupUi(this);

    ui_->text_edit->setVisible(!read_only_mode);
    ui_ ->text_label -> setVisible(read_only_mode);

    font_size=16;
    zoom(font_size);


    connect(ui_->zoom_in_text_button, SIGNAL(clicked()), this, SLOT(zoomTextIn()));
    connect(ui_->zoom_out_text_button, SIGNAL(clicked()), this, SLOT(zoomTextOut()));

}

TextWidget::TextWidget(const TextWidget& text_widget)
    : QWidget(text_widget.parentWidget())
    , ui_(new Ui::TextWidget)
{

}


TextWidget::~TextWidget()
{
    delete ui_;
}

void TextWidget::show(const QString &output, const QString)
{
    if ( ui_->text_edit->isVisible())
        ui_->text_edit->setText(output);
    else
        ui_-> text_label ->setText(output);
}

void TextWidget::setNull()
{
    ui_->text_edit->setText("");
}
bool TextWidget::saveToPackageFolder(const QString& some_path, const QString &deck_id_name)
{
    //not needed to implement for text widget
    return true;
}


bool TextWidget::isAdded() const
{
    return !ui_->text_edit->toPlainText().isNull();
}

QString TextWidget::getName() const
{
    return ui_->text_edit->toPlainText();
}


void TextWidget::zoomTextIn()
{
    if (font_size<FONT_SIZE_MAX)
    {
        font_size = font_size + FONT_SIZE_STEP;
        zoom(font_size);
        if (font_size>=FONT_SIZE_MAX)
            ui_->zoom_in_text_button->setEnabled(false);
        if (font_size>FONT_SIZE_MIN)
            ui_->zoom_out_text_button->setEnabled(true);
    }
}
void TextWidget::zoomTextOut()
{
    if (font_size>FONT_SIZE_MIN)
    {
        font_size = font_size - FONT_SIZE_STEP;
        zoom(font_size);
        if (font_size <= FONT_SIZE_MIN)
        {
            ui_->zoom_out_text_button->setEnabled(false);
        }
        if (font_size < FONT_SIZE_MAX)
        {
            ui_->zoom_in_text_button->setEnabled(true);
        }
    }
}

void TextWidget::zoom(const int &size)
{
    if ( ui_->text_edit->isVisible())
    {
        QFont font = QFont();
        font.setPointSize(size);
        ui_->text_edit->setFont(font);
    }
    else
    {
        QFont font = ui_-> text_label ->font();
        font.setPointSize(size);
        ui_ ->text_label ->setFont(font);
    }

}
