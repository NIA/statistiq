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

#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <QMainWindow>

namespace Ui {
class ReportWindow;
}

class ReportWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ReportWindow(QWidget *parent = 0);

    void setupForFile(QString fullName, QString shortName);
    void setContent(QString html);

    ~ReportWindow();

public slots:
    void sl_saveAs();
    void sl_print();

signals:
    void si_closed();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::ReportWindow *ui;

    QString defaultSaveFilename;
};

#endif // REPORTWINDOW_H
