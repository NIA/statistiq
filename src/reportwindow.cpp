#include "reportwindow.h"
#include "ui_reportwindow.h"
#include <QFileInfo>
#include <QCloseEvent>

ReportWindow::ReportWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReportWindow)
{
    ui->setupUi(this);
}

void ReportWindow::setupForFile(QString fullName, QString shortName) {
    setWindowTitle(tr("StatistiQ report for %1").arg(shortName));

    QFileInfo fi(fullName);
    defaultSaveFilename = fi.path() + fi.baseName() + ".html";
}

void ReportWindow::setContent(QString html) {
    ui->textBrowser->setHtml(html);
}

void ReportWindow::sl_saveAs() {
    // NOT YET IMPLEMENTED
}

void ReportWindow::sl_print() {
    // NOT YET IMPLEMENTED
}

void ReportWindow::closeEvent(QCloseEvent *) {
    emit si_closed();
}

ReportWindow::~ReportWindow()
{
    delete ui;
}
