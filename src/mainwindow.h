#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "statistic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void sl_quit();
    void sl_about();
    void sl_open();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *tableModel;

    void setupTable();
    QString formatStats(const Statistic &stat);
    static QList<QStandardItem*> dataToItems(QList<double> data);
};

#endif // MAINWINDOW_H
