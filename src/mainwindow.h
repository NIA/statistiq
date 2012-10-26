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
    void sl_dataUpdated();
    void sl_histogramUpdated();
    void sl_showHistogramTriggered(bool show);
    void sl_showDataCurveTriggered(bool show);
    void sl_showReport(bool show);
    void sl_reportClosed();

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    QSpinBox spinHistogramIntervals;
    QCheckBox checkHistogramFraction;
    ReportWindow * reportWindow;

    Statistic * stat;
    Histogram * histogram;
    QwtPlotCurve * curve;
    QwtPlotCurve * averageLine;

    void initHistogramControls();
    void initGrid(QwtPlot * plot);
    void initCurve();
    QString formatStats();
    QString formatReport();
};

#endif // MAINWINDOW_H
