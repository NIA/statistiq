#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QFileInfo>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.addLibraryPath( a.applicationDirPath() + "/plugins" );

    QTranslator translator;
    translator.load("statistiq_" + QLocale::system().name());
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
