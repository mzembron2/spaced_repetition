#include "SoundWidget.h"

SoundWidget::SoundWidget(QWidget *parent, bool read_only_mode)
    : QWidget(parent),
      ui_(new Ui::SoundWidget)
{
    ui_->setupUi(this);
    if (read_only_mode==true)
        ui_ ->open_button -> setVisible(false);


    media_player_ = new QMediaPlayer(this);
    media_player_->setAudioRole(QAudio::VideoRole);
    media_playlist_ = new QMediaPlaylist();
    media_player_->setPlaylist(media_playlist_);

    connect(ui_ -> open_button, SIGNAL(clicked()), this, SLOT(open()));
    connect(ui_ -> play_button, SIGNAL(clicked()), this, SLOT(play()));
    connect(ui_ -> stop_button, SIGNAL(clicked()), this, SLOT(stop()));

}
SoundWidget::~SoundWidget()
{
    delete ui_;
}
SoundWidget::SoundWidget(const SoundWidget& sound_question_widget)
    : QWidget(sound_question_widget.parentWidget())
    , ui_(new Ui::SoundWidget)
{

}

void SoundWidget::show(const QString &output_file, const QString deck_id)
{
    const QFileInfo fileInfo(__FILE__);
    const QDir main_dir = fileInfo.dir();
    const QString absolute_path = main_dir.absoluteFilePath("../Data/"+deck_id+"/"+output_file);

    addToPlaylist(QUrl::fromLocalFile(absolute_path));
}

QString SoundWidget::getName() const
{
    return media_name_;
}
bool SoundWidget::isAdded() const
{
    return !media_playlist_ -> isEmpty();
}

void SoundWidget::setNull()
{
    media_playlist_ -> clear();
    playlist_url_.clear();
    ui_ ->media_name_label -> setText("Nie wczytano pliku");
}

bool SoundWidget::isPlaylist(const QUrl &url) const
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    //becouse of our unability to reject other file in QDialog
    const std::vector<QString> supported_suffixs {"mp3", "wav", "ogg", "wma", "flac", "m3u"};
    return fileInfo.exists() && std::binary_search(supported_suffixs.begin(), supported_suffixs.end(), fileInfo.suffix());
}

void SoundWidget::addToPlaylist(const QUrl &url)
{
    media_playlist_ -> clear();
    if (isPlaylist(url))
    {
        media_playlist_->addMedia(url);
        makeImageName(url.toString());

        ui_ -> media_name_label -> setText("Wczytano: "+media_name_);
    }
    else
    {
        setNull();
        QMessageBox::warning(this, tr("Niewspierane rozszerzenie pliku"),
                                       tr("Wspierane rozszerzenia pliku to .mp3, .wav, .ogg, .wma, .flac, .m3u"));
    }

}

QString SoundWidget::makeImageName(const QString &fil_path)
{
    std::string str = fil_path.toStdString();
    str = str.substr(str.find_last_of("/\\") + 1);
    media_name_ = QString::fromUtf8(str.c_str());
    return media_name_;
}

bool SoundWidget::saveFile(const QString& path)
{
    return QFile::copy(playlist_url_.toLocalFile(), path);
}

bool SoundWidget::saveToPackageFolder(const QString &uniq_image_name, const QString &deck_id_name)
{
    const QFileInfo fileInfo(__FILE__);
    const QDir main_dir = fileInfo.dir();
    const QString absolute_path = main_dir.absoluteFilePath("../Data/"+deck_id_name+"/"+uniq_image_name);
    return saveFile(absolute_path);

}
//private slots start

void SoundWidget::open()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = media_player_->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
    {
        addToPlaylist(fileDialog.selectedUrls().constFirst());
        playlist_url_=fileDialog.selectedUrls().constFirst();
    }
}

void SoundWidget::play()
{
    if (!media_playlist_ -> isEmpty())
        media_playlist_ -> setCurrentIndex(1);
        media_player_->setPlaylist(media_playlist_);
        media_player_->play();
}

void SoundWidget::stop()
{
    if (!media_playlist_ -> isEmpty())
        media_player_->stop();
}
//private slots end
