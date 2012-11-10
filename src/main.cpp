/*
 * (C) Copyright 2012 Kuban State University (http://kubsu.ru/),
 *     Faculty of Physics and Technology, Physics and Information Systems Chair
 *
 * All rights reserved. This file is part of ProcessData, a program
 * for getting various statistical information from measurement data.
 *
 * Developed by Ivan Novikov (http://github.com/NIA)
 * under the direction of Leontiy Grigorjan
 *
 */

#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QFileInfo>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.addLibraryPath( a.applicationDirPath() + "/plugins" );

    QTranslator translator, qtTranslator;
    translator.load("processdata_" + QLocale::system().name());
    qtTranslator.load("qt_" + QLocale::system().name());
    a.installTranslator(&translator);
    a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
