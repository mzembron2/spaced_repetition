#include <gtest/gtest.h>
#include <QDir>
#include <QtSql>
#include <QDebug>
#include <iostream>
#include "../src/ImageWidget.h"


TEST(ImageWidgetTest, makeImageName) 
{
    int i =0 ;
    char *argv[1];
    QApplication a(i, argv);
    ImageWidget *testImageWidget = new ImageWidget();

    const QFileInfo fileInfo(__FILE__);
    const QDir main_dir = fileInfo.dir();

    const QString absolute_path = main_dir.absoluteFilePath("../DataTest/Cezary_Pazura.jpg");
    EXPECT_EQ(testImageWidget -> makeImageName(absolute_path),"Cezary_Pazura.jpg");
    
    delete testImageWidget;
}

TEST(ImageWidgetTest2, showAndDeleteImage) 
{
    int i =0 ;
    char *argv[1];
    QApplication a(i, argv);
    ImageWidget *testImageWidget = new ImageWidget();

    const QFileInfo fileInfo(__FILE__);
    const QDir main_dir = fileInfo.dir();

    const QString absolute_path = main_dir.absoluteFilePath("../DataTest/Cezary_Pazura.jpg");
    testImageWidget -> loadFile(absolute_path);
    EXPECT_EQ(testImageWidget -> isAdded(), true);
    EXPECT_EQ(testImageWidget -> getName(), "Cezary_Pazura.jpg");
    testImageWidget -> setNull();
    EXPECT_EQ(testImageWidget -> isAdded(), false);

  delete testImageWidget;
}