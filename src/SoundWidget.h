#ifndef SOUNDQUESTIONWIDGET_H
#define SOUNDQUESTIONWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include "FlashcardWidget.h"
#include "../UI/ui_soundwidget.h"

/// The class represents sound associated with the
/// flashcard (the sound could be a question or an
/// answer).
/// It provides methods for: loading the sound from
/// file, saving to it and playing via its GUI
/// interface.

namespace Ui {
    class SoundWidget;
}

class SoundWidget : public QWidget, public FlashcardWidget
{
    Q_OBJECT
public:
    SoundWidget(QWidget *parent = nullptr, bool read_only_mode=false);
    SoundWidget(const SoundWidget&);
    virtual ~SoundWidget();
    void show(const QString&, const QString);
    QString getName() const;
    bool isAdded() const;
    void setNull();
    void addToPlaylist(const QUrl &url);
    bool saveToPackageFolder(const QString&, const QString &deck_id_name);
    bool isPlaylist(const QUrl&) const;
    QString makeImageName(const QString &fil_path);

public slots:
    void stop();

private slots:
    void open();
    void play();

private:
    bool saveFile(const QString&);
    Ui::SoundWidget *ui_;
    QMediaPlayer *media_player_;
    QMediaPlaylist *media_playlist_;
    QMediaPlayer::State media_player_state_ = QMediaPlayer::StoppedState;
    QString media_name_;
    QUrl playlist_url_;
};

#endif // SOUNDQUESTIONWIDGET_H
