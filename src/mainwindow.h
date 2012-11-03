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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QCheckBox>
#include <qwt_plot_histogram.h>
#include <qwt_plot_curve.h>
#include "statistic.h"
#include "histogram.h"
#include "reportwindow.h"
#include "logwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    // Suppress the menu that hides the toolbar
    virtual QMenu * createPopupMenu () { return NULL; }
    ~MainWindow();

public slots:
    void sl_quit();
    void sl_about();
    void sl_open();
    void sl_new();
    void sl_statisticUpdated();
    void sl_histogramUpdated();
    void sl_showHistogramTriggered(bool show);
    void sl_showDataCurveTriggered(bool show);
    void sl_showReport(bool show);
    void sl_reportClosed();
    void sl_showLog(bool show);
    void sl_logClosed();
    void sl_save();
    void sl_saveAs();
    void sl_dataModified();
    void sl_addData();
    void sl_addDataFromFile();
    void sl_removeSelected();
    void sl_saveCurveAsImage();
    void sl_saveHistogramAsImage();

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    QSpinBox spinHistogramIntervals;
    QCheckBox checkHistogramFraction;
    ReportWindow * reportWindow;
    LogWindow * logWindow;

    Statistic * stat;
    Histogram * histogram;
    QwtPlotCurve * curve;
    QwtPlotCurve * averageLine;

    bool closingAllowed;
    bool askForClosing();
    void resetClosingFlag() { closingAllowed = false; }

    void initContextMenus();
    void initHistogramControls();
    void initGrid(QwtPlot * plot);
    void initCurve();
    bool setStatistic(Statistic * newStat);
    bool closeStatistic();
    QString formatStats();
    QString formatReport();
    void save(QString filePath);
    void open(QString filePath);
    void saveImage(QImage image);

    // Some const strings used in different places of MainWindow
    const QString untitledFileName;
    const QString titleWithFile;
    const QString fileFilter;
};

#endif // MAINWINDOW_H
