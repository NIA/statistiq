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
        statusBar()->showMessage(tr("Loaded %1 values, %2 skipped")
                                 .arg(data.count())
                                 .arg(reader.warningMessages().count()));
    } else {
        statusBar()->showMessage(reader.errorMessage());
    }
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
