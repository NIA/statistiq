#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reader.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QPen>
#include <qwt_series_data.h>
#include <qwt_plot_grid.h>

namespace {
    inline void setTips(QWidget * widget, QString tip) {
        widget->setToolTip(tip);
        widget->setStatusTip(tip);
    }
    inline void setTips(QWidget &widget, QString tip) { setTips(&widget, tip); }
    inline void setToolbarMargins(QWidget * widget) { widget->setContentsMargins(0, 0, 5, 0); }

    const QColor GRID_COLOR(128, 128, 128);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    stat(NULL), histogram(NULL)
{
    ui->setupUi(this);
    initHistogramControls();
    initGrid();
}

void MainWindow::initHistogramControls() {
    histogram = new Histogram(ui->histogramArea);

    QLabel * labelHistogram = new QLabel(tr("Histogram:"));
    setToolbarMargins(labelHistogram);
    setTips(labelHistogram, tr("Histogram display options"));

    spinHistogramIntervals.setMinimum(2);
    spinHistogramIntervals.setValue(10);
    spinHistogramIntervals.setMaximum(100);
    setTips(spinHistogramIntervals, tr("Number of histogram intervals"));

    QLabel * labelColumns = new QLabel(tr("(columns)"));
    setToolbarMargins(labelColumns);
    setTips(labelColumns, tr("Number of histogram intervals"));

    checkHistogramFraction.setChecked(false);
    checkHistogramFraction.setText(tr("Fraction"));
    setTips(checkHistogramFraction, tr("Show fraction instead of values in histogram"));

    ui->mainToolBar->addWidget(labelHistogram);
    ui->mainToolBar->addWidget(&spinHistogramIntervals);
    ui->mainToolBar->addWidget(labelColumns);
    ui->mainToolBar->addWidget(&checkHistogramFraction);
}

void MainWindow::initGrid() {
    QwtPlotGrid * grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(GRID_COLOR));
    grid->setMinPen(QPen(GRID_COLOR, 1, Qt::DashLine));
    grid->attach(ui->histogramArea);
}

void MainWindow::sl_open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open data file"), ".",
                                                    tr("Plain text file (*.txt *.csv *.dat)"));
    if(fileName.isEmpty()) {
        return;
    }
    Reader reader(this, fileName);
    if(reader.isValid()) {
        if(stat != NULL) {
            delete stat;
        }
        setWindowTitle(tr("%1 - StatistiQ - a data processing utility").arg(reader.shortFileName()));
        stat = new Statistic(this, reader.data(), reader.formatFileInfo());
        ui->dataTable->setModel(stat->itemModel());

        stat->setHistogramIntervalsNumber(spinHistogramIntervals.value());
        stat->setHistogramFraction(checkHistogramFraction.isChecked());
        connect(stat, SIGNAL(si_statisticChanged()), SLOT(sl_dataUpdated()));
        connect(stat, SIGNAL(si_histogramChanged()), SLOT(sl_histogramUpdated()));
        connect(&spinHistogramIntervals, SIGNAL(valueChanged(int)), stat, SLOT(sl_histogramIntervalsChanged(int)));
        connect(&checkHistogramFraction, SIGNAL(stateChanged(int)), stat, SLOT(sl_histogramFractionChanged(int)));
        sl_dataUpdated();
        sl_histogramUpdated();
    }
    statusBar()->showMessage(reader.formatReport());
}

void MainWindow::sl_dataUpdated() {
    ui->infoText->setHtml(formatStats());
}

void MainWindow::sl_histogramUpdated() {
    histogram->setData(new QwtIntervalSeriesData(stat->histogramSamples()));
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
    delete histogram;
    delete ui;
}
