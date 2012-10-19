#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reader.h"

#include <QMessageBox>
#include <QFileDialog>
#include <qwt_series_data.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), stat(NULL)
{
    ui->setupUi(this);
    histogram.attach(ui->histogramArea);
    histogram.setStyle(QwtPlotHistogram::Columns);
    histogram.setBrush(QBrush(QColor(0, 200, 30)));
}

void MainWindow::sl_open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open data file"), ".",
                                                    tr("Plain text file (*.txt *.csv *.dat)"));
    Reader reader(this, fileName);
    if(reader.isValid()) {
        if(stat != NULL) {
            delete stat;
        }
        setWindowTitle(tr("%1 - StatistiQ - a data processing utility"));
        stat = new Statistic(this, reader.data(), reader.formatFileInfo());
        ui->dataTable->setModel(stat->itemModel());
        connect(stat, SIGNAL(si_statisticChanged()), SLOT(sl_dataUpdated()));
        sl_dataUpdated();
    }
    statusBar()->showMessage(reader.formatReport());
}

void MainWindow::sl_dataUpdated() {
    ui->infoText->setHtml(formatStats());
    histogram.setData(new QwtIntervalSeriesData(stat->histogramSamples()));
    ui->histogramArea->replot();
}

QString MainWindow::formatStats() {
    if(stat == NULL) {
        return "";
    }
    return tr("<h2>%header</h2><p/>"
              "<p><b>Number:</b> %number</p>"
              "<p><b>Min/Max:</b> %min ... %max</p>"
              "<p><b>Average:</b> %avg</p>"
              "<p><b>Dispersion:</b> %disp</p>"
              "<p><b>Std. deviation:</b> %stddev</p>"
              "<p><b>3<sup>rd</sup> Moment:</b> %moment3</p>"
              "<p><b>4<sup>th</sup> Moment:</b> %moment4</p>")
    .replace("%header", stat->header())
    .replace("%number", stat->numberStr())
    .replace("%min", stat->minStr()).replace("%max", stat->maxStr())
    .replace("%avg", stat->averageStr())
    .replace("%disp", stat->dispersionStr())
    .replace("%stddev", stat->stdDeviationStr())
    .replace("%moment3", stat->thirdMomentStr())
    .replace("%moment4", stat->fourthMomentStr());
}

void MainWindow::sl_quit() {
    QApplication::quit();
}

void MainWindow::sl_about() {
    QMessageBox::about(this, tr("About StatistiQ"),
            tr("StatistiQ is an app for getting various \n"
               "statistical information from experiment data\n\n"
               "(c) 2012, Faculty of Physics and Technology, Kuban State University\n"
               "written by Ivan Novikov, lead by L. R. Grigorjan"));

}

MainWindow::~MainWindow() {
    delete ui;
}
