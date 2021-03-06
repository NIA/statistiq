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
#include <QProcess>
#include <QPainter>
#include <qwt_series_data.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_scaleitem.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_renderer.h>

namespace {
    inline void setTips(QWidget * widget, QString tip) {
        widget->setToolTip(tip);
        widget->setStatusTip(tip);
    }
    inline void setTips(QWidget &widget, QString tip) { setTips(&widget, tip); }
    inline void setToolbarMargins(QWidget * widget) { widget->setContentsMargins(0, 0, 5, 0); }

    inline QImage widgetToImage(QWidget * w) {
        QImage img(w->size(), QImage::Format_ARGB32);
        img.fill(Qt::transparent);
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

    inline void rotateAxisLabel(QwtScaleDraw * axisDraw) {
        axisDraw->setLabelRotation(-90);
        axisDraw->setLabelAlignment(Qt::AlignHCenter | Qt::AlignTop);
    }

    inline void setAxisFontSize(QwtPlot * plot, QwtPlot::Axis axisId, int size) {
        QFont axisFont = plot->axisFont(axisId);
        axisFont.setPointSize(size);
        plot->setAxisFont(axisId, axisFont);
    }

    const QColor GRID_COLOR(128, 128, 128);
    const QColor CURVE_COLOR(40, 90, 120);
    const QColor CURVE_FILL(10, 160, 255, 100);
    const QColor AVERAGE_COLOR(255, 50, 50);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), reportWindow(new ReportWindow(NULL)), logWindow(new LogWindow(NULL)),
    stat(NULL), histogram(NULL), closingAllowed(false),

    untitledFileName(tr("untitled.csv")), titleWithFile(tr("%1[*] - StatistiQ - a data processing utility")),
    fileFilter(tr("Plain text file (*.csv *.txt *.dat);;All files (*.*)"))
{
    Logger::info(tr("Application started"));

    ui->setupUi(this);
    initHistogramControls();
    initCurve();
    initContextMenus();

    connect(reportWindow, SIGNAL(si_closed()), SLOT(sl_reportClosed()));
    connect(logWindow, SIGNAL(si_closed()), SLOT(sl_logClosed()));

    QStringList arguments = QApplication::arguments();
    arguments.removeAt(0); // ignore own path
    if(arguments.size() > 0) {
        open(arguments[0]);
        // Open new instances for each of the other files
        for(int i = 1; i < arguments.size(); ++i) {
            QProcess::startDetached(QApplication::applicationFilePath(), arguments.mid(i, 1));
        }
    }
}

void MainWindow::initContextMenus() {
    ui->dataTable->addAction(ui->actionAddData);
    ui->dataTable->addAction(ui->actionAddFromFile);
    ui->dataTable->addAction(ui->actionRemoveSelected);
    ui->dataTable->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->curveArea->addAction(ui->actionSavePlotAsImage);
    ui->curveArea->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->histogramArea->addAction(ui->actionSaveHistogramAsImage);
    ui->histogramArea->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::initHistogramControls() {
    initGrid(ui->histogramArea);
    rotateAxisLabel(ui->histogramArea->axisScaleDraw(QwtPlot::yLeft));
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
    rotateAxisLabel(ui->curveArea->axisScaleDraw(QwtPlot::yLeft));
    ui->curveArea->setVisible(ui->actionShowDataCurve->isChecked());
    new TooltipPlotPicker(ui->curveArea);
    connect(ui->actionShowDataCurve, SIGNAL(triggered(bool)), SLOT(sl_showDataCurveTriggered(bool)));

    curve = new QwtPlotCurve;
    curve->setBrush(CURVE_FILL);
    curve->setPen(CURVE_COLOR);
    curve->setOrientation(Qt::Vertical);
    curve->attach(ui->curveArea);

    ui->curveArea->setAxisMaxMajor(QwtPlot::yLeft, 4);

    averageLine = new QwtPlotCurve;
    averageLine->setPen(AVERAGE_COLOR);
    averageLine->attach(ui->curveArea);
}

void MainWindow::sl_open() {
    QString supposedPath = ".";
    if(stat != NULL) {
        supposedPath =  QFileInfo(stat->filePath()).path();
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open data file"), supposedPath, fileFilter);
    if(fileName.isEmpty()) {
        return;
    } else {
        open(fileName);
    }
}

void MainWindow::open(QString fileName) {
    if(false == askForClosing()) {
        return;
    }

    Reader reader(this, fileName);
    if(reader.isValid()) {
        if( setStatistic(new Statistic(this, reader.data(), reader.formatFileInfo())) ) {
            stat->setFilePath(fileName);

            setWindowTitle(titleWithFile.arg(reader.shortFileName()));
            reportWindow->setupForFile(fileName, reader.shortFileName());
        }
    } else {
        QMessageBox::warning(this, tr("Failed to open file"), reader.errorMessage());
    }
    statusBar()->showMessage(reader.formatReport());
    resetClosingFlag();
}

void MainWindow::sl_addDataFromFile() {
    if(stat == NULL) {
        return;
    }

    QFileInfo fi(stat->filePath());
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file to add to statistic"), fi.path(), fileFilter);
    if(fileName.isEmpty()) {
        return;
    }
    Reader reader(this, fileName);
    if(reader.isValid()) {
        stat->append(reader.data());
    } else {
        QMessageBox::warning(this, tr("Failed to open file"), reader.errorMessage());
    }
    statusBar()->showMessage(reader.formatReport());
}

void MainWindow::sl_new() {
    NewStatDialog * dialog = new NewStatDialog(this, NewStatDialog::ReplaceWithNew);
    if(dialog->exec() == QDialog::Accepted) {
        if( setStatistic(new Statistic(this, dialog->data(), untitledFileName)) ) {
            setWindowTitle(titleWithFile.arg(untitledFileName));

            stat->setModified(true);
            reportWindow->setupForFile(untitledFileName, untitledFileName);
            QString message = tr("Successfully created new statistic of %1 items").arg(dialog->data().size());
            statusBar()->showMessage(message);
            Logger::info(message);
        }
    }
}

void MainWindow::sl_addData() {
    if(stat == NULL) {
        return;
    }

    NewStatDialog * dialog = new NewStatDialog(this, NewStatDialog::AppendToExisting);
    if(dialog->exec() == QDialog::Accepted) {
        stat->append(dialog->data());
        QString message = tr("Successfully appended %1 items to current statistic").arg(dialog->data().size());
        statusBar()->showMessage(message);
        Logger::info(message);
    }
}

void MainWindow::sl_removeSelected() {
    if(stat == NULL) {
        return;
    }

    QModelIndexList selected = ui->dataTable->selectionModel()->selectedRows();
    stat->remove(selected);
}

bool MainWindow::setStatistic(Statistic *newStat) {
    if( ! closeStatistic() ) {
        return false;
    }

    stat = newStat;

    ui->dataTable->setModel(stat->itemModel());
    ui->actionShowReport->setEnabled(true);
    ui->actionSave->setEnabled(false);
    ui->actionSaveAs->setEnabled(true);
    ui->actionAddData->setEnabled(true);
    ui->actionAddFromFile->setEnabled(true);
    ui->actionRemoveSelected->setEnabled(true);

    stat->setHistogramIntervalsNumber(spinHistogramIntervals.value());
    stat->setHistogramFraction(checkHistogramFraction.isChecked());
    connect(stat, SIGNAL(si_statisticChanged()), SLOT(sl_statisticUpdated()));
    connect(stat, SIGNAL(si_histogramChanged()), SLOT(sl_histogramUpdated()));
    connect(stat, SIGNAL(si_modified()),         SLOT(sl_dataModified()));
    connect(&spinHistogramIntervals, SIGNAL(valueChanged(int)), stat, SLOT(sl_histogramIntervalsChanged(int)));
    connect(&checkHistogramFraction, SIGNAL(stateChanged(int)), stat, SLOT(sl_histogramFractionChanged(int)));
    sl_statisticUpdated();
    sl_histogramUpdated();

    return true;
}

bool MainWindow::askForClosing() {
    if(stat != NULL && stat->isModified()) {
        QMessageBox::StandardButton answer =
                QMessageBox::question(this, tr("Save before closing?"),
                tr("Your current statistic has unsaved changes.\nWould you like to save them?"),
                QMessageBox::StandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel),
                QMessageBox::Save);
        if(answer == QMessageBox::Cancel) {
            Logger::info(tr("Closing unsaved statistic canceled"));
            closingAllowed = false;
        } else if(answer == QMessageBox::Save) {
            sl_save();
            closingAllowed = true;
        } else {
            Logger::warning(tr("Statistic '%1' closed without saving").arg(stat->header()));
            closingAllowed = true;
        }
    } else {
        closingAllowed = true;
    }
    return closingAllowed;
}

/* Returns true if statistic is closed, false if closing was canceled by user */
bool MainWindow::closeStatistic() {
    if(stat == NULL) { return true; }

    if(stat->isModified()) {
        if( ! closingAllowed ) {
            // It means that we didn't ask yet, so ask
            if( false == askForClosing() ) {
                return false;
            }
        }
    }
    setWindowModified(false);
    delete stat;
    resetClosingFlag(); // Return to initial state
    return true;
}

void MainWindow::sl_save() {
    if( ! stat->isModified() ) {
        return;
    }
    if( stat->filePath().isEmpty()) {
        // A new, unsaved yet statistic
        sl_saveAs();
    } else {
        save(stat->filePath());
    }
}

void MainWindow::sl_saveAs() {
    QString supposedFileName = stat->filePath();
    if(supposedFileName.isEmpty()) { supposedFileName = untitledFileName; }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save statistic to data file"), supposedFileName, fileFilter);
    if(fileName.isEmpty()) {
        return;
    }
    save(fileName);
}

void MainWindow::save(QString filePath) {
    // Write
    Writer writer(this, filePath);
    if(writer.isValid()) {
        writer.write(stat->rawData());
    } else {
        QMessageBox::warning(this, tr("Save statistic error"), writer.errorMessage());
        return;
    }

    // Update window display & controls
    QString shortFileName = Reader::toShortFileName(filePath);
    statusBar()->showMessage(tr("Successfully saved statistic to '%1'").arg(shortFileName));
    ui->actionSave->setEnabled(false);
    setWindowTitle(titleWithFile.arg(shortFileName));
    setWindowModified(false);

    // Update statistic properties
    QString newHeader = Reader::formatFileInfo(shortFileName, QDateTime::currentDateTime());
    stat->setHeader(newHeader);
    stat->setFilePath(filePath);
    stat->setModified(false);
}

void MainWindow::sl_saveCurveAsImage() {
    saveImage(widgetToImage(ui->curveArea));
}

void MainWindow::sl_saveHistogramAsImage() {
    saveImage(widgetToImage(ui->histogramArea));
}

void MainWindow::saveImage(QImage image) {
    QString supposedFileName = tr("image.png");
    if(stat != NULL) {
        QFileInfo fi(stat->filePath());
        supposedFileName = fi.dir().filePath(supposedFileName);
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save image to file"), supposedFileName,
                                                    tr("Portable Network Graphics (*.png);;JPEG (*.jpg *.jpeg);;Windows Bitmap (*.bmp)"));
    if(fileName.isEmpty()) {
        return;
    }
    QString shortFileName = Reader::toShortFileName(fileName);
    if( true == image.save(fileName) ) {
        ui->statusBar->showMessage(tr("Successfully saved image to '%1'").arg(shortFileName));
        Logger::info(tr("Successfully saved image to '%1'").arg(fileName));
    } else {
        Logger::error(tr("Failed to save image to '%1'").arg(fileName));
        QMessageBox::warning(this, tr("Save image error"), tr("Failed to save image to '%1'.\nTry choosing another file location and/or format").arg(fileName));
    }
}

void MainWindow::sl_statisticUpdated() {
    ui->infoText->setHtml(formatStats());

    curve->setSamples(stat->dataPoints());
    curve->setBaseline(stat->average());
    averageLine->setSamples(horizontalLine(1, stat->number(), stat->average()));
    ui->curveArea->replot();

    reportWindow->setContent(formatReport());
}

void MainWindow::sl_dataModified() {
    setWindowModified(true);
    ui->actionSave->setEnabled(true);
    Logger::trace(tr("Data modified"));
}

void MainWindow::sl_histogramUpdated() {
    histogram->setData(new QwtIntervalSeriesData(stat->histogramSamples()));
    ui->histogramArea->replot();

    Logger::trace(tr("Histogram updated"));
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

    return tr("<center><h2>%header</h2>"
              "<p><big>x = %avg &plusmn; %stddev</big></p></center>"
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

void MainWindow::sl_showLog(bool show) {
    if(show) {
        logWindow->show();
    } else {
        logWindow->hide();
    }
}

void MainWindow::sl_reportClosed() {
    ui->actionShowReport->setChecked(false);
}

void MainWindow::sl_logClosed() {
    ui->actionShowLog->setChecked(false);
}

void MainWindow::sl_quit() {
    close();
}

void MainWindow::sl_about() {
    QMessageBox::about(this, tr("About StatistiQ"),
            tr("StatistiQ version 1.0\n\n"
               "StatistiQ is an app for getting various \n"
               "statistical information from experiment data\n\n"
               "(c) 2012, Physics and Information Systems Chair,\n"
               "Faculty of Physics and Technology, Kuban State University"));

}

void MainWindow::closeEvent(QCloseEvent *e) {
    if( closeStatistic() ) {
        reportWindow->close();
        logWindow->close();
        e->accept();
    } else {
        e->ignore();
    }
}

MainWindow::~MainWindow() {
    delete histogram;
    delete curve;
    delete ui;
}
