#include "reportwindow.h"
#include "ui_reportwindow.h"
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
    setWindowTitle(tr("StatistiQ report for %1").arg(shortName));

    QFileInfo fi(fullName);
    defaultSaveFilename = fi.dir().filePath(fi.baseName() + "_report.html");
}

void ReportWindow::setContent(QString html) {
    ui->textBrowser->setHtml(html);
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
        QMessageBox::warning(this, tr("Save report error"),
                             tr("Cannot save to file\n'%1'\nError code %2").arg(fileName).arg(errcode));
    } else {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << ui->textBrowser->document()->toHtml("utf-8");
        file.close();
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
}

void ReportWindow::closeEvent(QCloseEvent *) {
    emit si_closed();
}

ReportWindow::~ReportWindow()
{
    delete ui;
}
