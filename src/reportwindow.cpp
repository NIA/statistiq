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

#include "reportwindow.h"
#include "ui_reportwindow.h"
#include "logger.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>

ReportWindow::ReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReportWindow)
{
    ui->setupUi(this);
}

void ReportWindow::setupForFile(QString fullName, QString shortName) {
    setWindowTitle(tr("ProcessData report for %1").arg(shortName));

    QFileInfo fi(fullName);
    defaultSaveFilename = fi.dir().filePath(fi.baseName() + "_report.html");
}

void ReportWindow::setContent(QString html) {
    ui->textBrowser->setHtml(html);
    Logger::info(tr("Report content updated"));
}

void ReportWindow::sl_saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save report as HTML"), defaultSaveFilename,
                                                    tr("HTML file (*.html *.htm)"));

    if(fileName.isEmpty()) {
        return;
    }
    QFile file(fileName, this);
    if( ! file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
        QFile::FileError errcode = file.error();
        Logger::error(tr("Failed to save report to file '%1'. Error code %2.").arg(fileName).arg(errcode));
        QMessageBox::warning(this, tr("Save report error"),
                             tr("Cannot save to file\n'%1'\nError code %2").arg(fileName).arg(errcode));
    } else {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << ui->textBrowser->document()->toHtml("utf-8");
        file.close();
        Logger::info(tr("Successfully saved report to file '%1'").arg(fileName));
    }
}

void ReportWindow::sl_print() {
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print report"));
    if (ui->textBrowser->textCursor().hasSelection()) {
        dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    }
    if (dialog->exec() != QDialog::Accepted) {
        return;
    }
    ui->textBrowser->print(&printer);
    Logger::info(tr("Successfully printed report to printer '%1'").arg(printer.printerName()));
}

void ReportWindow::closeEvent(QCloseEvent *) {
    emit si_closed();
}

ReportWindow::~ReportWindow()
{
    delete ui;
}
