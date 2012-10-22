#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QCheckBox>
#include <qwt_plot_histogram.h>
#include "statistic.h"
#include "histogram.h"

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

private:
    Ui::MainWindow *ui;
    QSpinBox spinHistogramIntervals;
    QCheckBox checkHistogramFraction;

    Statistic * stat;
    Histogram * histogram;

    void initHistogramControls();
    void initGrid();
    QString formatStats();
};

#endif // MAINWINDOW_H
