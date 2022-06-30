#include <gtest/gtest.h>
#include <QDir>
#include <QtSql>
#include <QDebug>
#include <iostream>
#include "../src/SoundWidget.h"


TEST(SoundWidgetTest, IsPlaylist) 
{
    int i =0 ;
    char *argv[1];
    QApplication a(i, argv);
    SoundWidget *soundWidget = new SoundWidget();

    const QFileInfo fileInfo(__FILE__);
    const QDir main_dir = fileInfo.dir();

    const QString absolute_path = main_dir.absoluteFilePath("../DataTest/Cezary_Pazura.jpg");
    EXPECT_EQ(soundWidget -> isPlaylist(QUrl::fromLocalFile(absolute_path)), false);

    const QString absolute_path2 = main_dir.absoluteFilePath("../DataTest/file_example_MP3_700KB.mp3");
    EXPECT_EQ(soundWidget -> isPlaylist(QUrl::fromLocalFile(absolute_path2)), true);

    delete soundWidget;
}


TEST(SoundWidgetTest2, showAndDeleteSound) 
{
    int i =0 ;
    char *argv[1];
    QApplication a(i, argv);
    SoundWidget *soundWidget = new SoundWidget();

    const QFileInfo fileInfo(__FILE__);
    const QDir main_dir = fileInfo.dir();


    const QString absolute_path2 = main_dir.absoluteFilePath("../DataTest/file_example_MP3_700KB.mp3");
    soundWidget -> addToPlaylist(QUrl::fromLocalFile(absolute_path2));
    EXPECT_EQ(soundWidget -> isAdded(), true);
    EXPECT_EQ(soundWidget -> getName(), "file_example_MP3_700KB.mp3");
    soundWidget -> setNull();
    EXPECT_EQ(soundWidget -> isAdded(), false);
    delete soundWidget;
}