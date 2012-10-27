#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reader.h"
#include "tooltipplotpicker.h"
#include "newstatdialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QPen>
#include <QBuffer>
#include <QCloseEvent>
#include <qwt_series_data.h>
#include <qwt_plot_grid.h>

namespace {
    inline void setTips(QWidget * widget, QString tip) {
        widget->setToolTip(tip);
        widget->setStatusTip(tip);
    }
    inline void setTips(QWidget &widget, QString tip) { setTips(&widget, tip); }
    inline void setToolbarMargins(QWidget * widget) { widget->setContentsMargins(0, 0, 5, 0); }

    inline QImage widgetToImage(QWidget * w) {
        QImage img(w->size(), QImage::Format_ARGB32);
        w->render(&img, QPoint(), QRegion(), QWidget::RenderFlags(QWidget::DrawChildren));
        return img;
    }

    inline QString image2base64(const QImage &img) {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        img.save(&buffer, "PNG"); // writes the image in PNG format inside the buffer
        return QString::fromLatin1(byteArray.toBase64().data());
    }

    inline QVector<QPointF> horizontalLine(double from, double to, double value) {
        QVector<QPointF> res;
        res << QPointF(from, value);
        res << QPointF(to, value);
        return res;
    }

    const QColor GRID_COLOR(128, 128, 128);
    const QColor CURVE_COLOR(40, 90, 120);
    const QColor CURVE_FILL(10, 160, 255, 100);
    const QColor AVERAGE_COLOR(255, 50, 50);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), reportWindow(new ReportWindow(NULL)),
    stat(NULL), histogram(NULL)
{
    ui->setupUi(this);
    initHistogramControls();
    initCurve();

    connect(reportWindow, SIGNAL(si_closed()), SLOT(sl_reportClosed()));
}

void MainWindow::initHistogramControls() {
    initGrid(ui->histogramArea);
    ui->histogramArea->setVisible(ui->actionShowHistogram->isChecked());
    new TooltipPlotPicker(ui->histogramArea);

    histogram = new Histogram(ui->histogramArea);
    connect(ui->actionShowHistogram, SIGNAL(triggered(bool)), SLOT(sl_showHistogramTriggered(bool)));

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

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(labelHistogram);
    ui->mainToolBar->addWidget(&spinHistogramIntervals);
    ui->mainToolBar->addWidget(labelColumns);
    ui->mainToolBar->addWidget(&checkHistogramFraction);
}

void MainWindow::initGrid(QwtPlot *plot) {
    QwtPlotGrid * grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(GRID_COLOR));
    grid->setMinPen(QPen(GRID_COLOR, 1, Qt::DashLine));
    grid->attach(plot);
}

void MainWindow::initCurve() {
    initGrid(ui->curveArea);
    new TooltipPlotPicker(ui->curveArea);
    ui->curveArea->setVisible(ui->actionShowDataCurve->isChecked());
    connect(ui->actionShowDataCurve, SIGNAL(triggered(bool)), SLOT(sl_showDataCurveTriggered(bool)));

    curve = new QwtPlotCurve;
    curve->setBrush(CURVE_FILL);
    curve->setPen(CURVE_COLOR);
    curve->setOrientation(Qt::Vertical);
    curve->attach(ui->curveArea);
    ui->curveArea->enableAxis(QwtPlot::yLeft, false);
    ui->curveArea->enableAxis(QwtPlot::xBottom, false);

    averageLine = new QwtPlotCurve;
    averageLine->setPen(AVERAGE_COLOR);
    averageLine->attach(ui->curveArea);
}

void MainWindow::sl_open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open data file"), ".",
                                                    tr("Plain text file (*.txt *.csv *.dat)"));
    if(fileName.isEmpty()) {
        return;
    }
    Reader reader(this, fileName);
    if(reader.isValid()) {
        setWindowTitle(tr("%1 - StatistiQ - a data processing utility").arg(reader.shortFileName()));
        reportWindow->setupForFile(fileName, reader.shortFileName());

        setStatistic(new Statistic(this, reader.data(), reader.formatFileInfo()));
    }
    statusBar()->showMessage(reader.formatReport());
}

void MainWindow::sl_new() {
    NewStatDialog * dialog = new NewStatDialog(this);
    if(dialog->exec() == QDialog::Accepted) {
        QString untitledFileName = tr("untitled.csv");
        setWindowTitle(tr("%1 - StatistiQ - a data processing utility").arg(untitledFileName));
        reportWindow->setupForFile(untitledFileName, untitledFileName);

        setStatistic(new Statistic(this, dialog->data(), untitledFileName));
    }
}

void MainWindow::setStatistic(Statistic *newStat) {
    if(stat != NULL) {
        // TODO: closeStatistic() method that will ask for saving
        delete stat;
    }
    stat = newStat;

    ui->dataTable->setModel(stat->itemModel());
    ui->actionShowReport->setEnabled(true);

    stat->setHistogramIntervalsNumber(spinHistogramIntervals.value());
    stat->setHistogramFraction(checkHistogramFraction.isChecked());
    connect(stat, SIGNAL(si_statisticChanged()), SLOT(sl_dataUpdated()));
    connect(stat, SIGNAL(si_histogramChanged()), SLOT(sl_histogramUpdated()));
    connect(&spinHistogramIntervals, SIGNAL(valueChanged(int)), stat, SLOT(sl_histogramIntervalsChanged(int)));
    connect(&checkHistogramFraction, SIGNAL(stateChanged(int)), stat, SLOT(sl_histogramFractionChanged(int)));
    sl_dataUpdated();
    sl_histogramUpdated();
}

void MainWindow::sl_dataUpdated() {
    ui->infoText->setHtml(formatStats());

    curve->setSamples(stat->dataPoints());
    curve->setBaseline(stat->average());
    averageLine->setSamples(horizontalLine(0, stat->number(), stat->average()));
    ui->curveArea->replot();

    reportWindow->setContent(formatReport());
}

void MainWindow::sl_histogramUpdated() {
    histogram->setData(new QwtIntervalSeriesData(stat->histogramSamples()));
    ui->histogramArea->replot();

    reportWindow->setContent(formatReport());
}

void MainWindow::sl_showHistogramTriggered(bool show) {
    ui->histogramArea->setVisible(show);
}

void MainWindow::sl_showDataCurveTriggered(bool show) {
    ui->curveArea->setVisible(show);
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

QString MainWindow::formatReport() {
    QString html = formatStats();
    html += QString("<div><img src='data:image/png;base64,%1'/></div><p/>").arg(image2base64(widgetToImage(ui->histogramArea)));
    html += stat->dataAsHtmlTable(7);
    return html;
}

void MainWindow::sl_showReport(bool show) {
    if(stat == NULL) {
        return;
    }
    if(show) {
        reportWindow->show();
    } else {
        reportWindow->hide();
    }
}

void MainWindow::sl_reportClosed() {
    ui->actionShowReport->setChecked(false);
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

void MainWindow::closeEvent(QCloseEvent *e) {
    // TODO: check if file saved
    reportWindow->close();
    e->accept();
}

MainWindow::~MainWindow() {
    delete histogram;
    delete curve;
    delete ui;
}
