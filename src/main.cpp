#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QFileInfo>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.addLibraryPath( a.applicationDirPath() + "/plugins" );

    QTranslator translator, qtTranslator;
    translator.load("statistiq_" + QLocale::system().name());
    qtTranslator.load("qt_" + QLocale::system().name());
    a.installTranslator(&translator);
    a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
