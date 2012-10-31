/*
 * (C) Copyright 2012 Kuban State University (http://kubsu.ru/),
 *     Faculty of Physics and Technology, Physics and Information Systems Chair
 *
 * All rights reserved. This file is part of StatistiQ, a program
 * for getting various statistical information from measurement data.
 *
 * Developed by Ivan Novikov (http://github.com/NIA)
 * under the direction of Leontiy Grigorjan
 *
 */

#ifndef NEWSTATDIALOG_H
#define NEWSTATDIALOG_H

#include <QDialog>

namespace Ui {
class NewStatDialog;
}

class NewStatDialog : public QDialog
{
    Q_OBJECT
    typedef QDialog super;
public:
    enum Purpose {
        ReplaceWithNew,
        AppendToExisting
    };

    explicit NewStatDialog(QWidget *parent = 0, Purpose purpose = ReplaceWithNew);

    QList<double> data() { return data_; }

    ~NewStatDialog();

public slots:
    void accept();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::NewStatDialog *ui;

    QList<double> data_;
};

#endif // NEWSTATDIALOG_H
