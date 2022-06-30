#include <QApplication>
#include <QDir>
#include <QtSql>
#include "MainWindow.h"
#include "DatabaseController.h"


int main(int argc, char *argv[]) {
    const QFileInfo fileInfo(__FILE__);
    const QDir mainDir = fileInfo.dir();
    const QString absolutePath = mainDir.absoluteFilePath("../Database/ProbaDataBase.db");
    QApplication a(argc, argv);

    DatabaseController db_contoller(0);
    db_contoller.connectToSQLiteDatabaseRequested(absolutePath);

    MainWindow w(0, &db_contoller);
    w.show();
    return a.exec();
}
