#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reader.h"

#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupTable();
}

void MainWindow::sl_open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open data file"), ".",
                                                    tr("Plain text file (*.txt *.csv *.dat)"));
    Reader reader(this, fileName);
    if(reader.isValid()) {
        QList<double> data = reader.data();
        // TODO: not insert, but replace!
        tableModel->insertColumn(0, dataToItems(data));

        // TODO: field, signal-slot connection
        Statistic stat(this, data);
        ui->infoText->setHtml(formatStats(stat));
    }
    statusBar()->showMessage(reader.formatReport());
}

void MainWindow::setupTable() {
    tableModel = new QStandardItemModel(0,1,this); //2 Rows and 1 Column

    tableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Value")));

    ui->dataTable->setModel(tableModel);
}

QList<QStandardItem*> MainWindow::dataToItems(QList<double> data) {
    QList<QStandardItem*> result;
    foreach(double d, data) {
        result.append(new QStandardItem(QString::number(d)));
    }
    return result;
}

QString MainWindow::formatStats(const Statistic &stat) {
    return tr("<p><b>Number:</b> %1</p>"
              "<p><b>Average:</b> %2</p>"
              "<p><b>Dispersion:</b> %3</p>"
              "<p><b>Std. deviation:</b> %4</p>")
    .arg(stat.number()).arg(stat.average())
    .arg(stat.dispersion()).arg(stat.stdDeviation());
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
