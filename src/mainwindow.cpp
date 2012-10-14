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
        QList<double> data = reader.data();

        if(stat != NULL) {
            delete stat;
        }
        // TODO: store file and date info, show in infobox and title
        stat = new Statistic(this, data);
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
    return tr("<p><b>Number:</b> %1</p>"
              "<p><b>Min/Max:</b> %2 ... %3</p>"
              "<p><b>Average:</b> %4</p>"
              "<p><b>Dispersion:</b> %5</p>"
              "<p><b>Std. deviation:</b> %6</p>")
    .arg(stat->number()).arg(stat->min()).arg(stat->max())
    .arg(stat->average())
    .arg(stat->dispersion()).arg(stat->stdDeviation());
}

void MainWindow::sl_quit() {
    QApplication::quit();
}

void MainWindow::sl_about() {
    QMessageBox::about(this, tr("About StatistiQ"),
            tr("StatistiQ is an app for getting various \n"
               "statistical information from experiment data\n\n"
               "(c) 2012, Faculty of Physics and Technology, Kuban State University\n"
               "written by Ivan Novikov"));

}

MainWindow::~MainWindow() {
    delete ui;
}
