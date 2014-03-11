#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString locale = QLocale::system().name().section('_', 0, 0);

    QTranslator qtTranslator;
    qtTranslator.load("l10n/hip_" + locale);
    a.installTranslator(&qtTranslator);

    //qDebug() << locale << QLocale::system().name();

    MainWindow w;
    w.show();
    
    return a.exec();
}
