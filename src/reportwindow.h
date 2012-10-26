#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <QMainWindow>

namespace Ui {
class ReportWindow;
}

class ReportWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ReportWindow(QWidget *parent = 0);

    void setupForFile(QString fullName, QString shortName);
    void setContent(QString html);

    ~ReportWindow();

public slots:
    void sl_saveAs();
    void sl_print();

signals:
    void si_closed();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::ReportWindow *ui;

    QString defaultSaveFilename;
};

#endif // REPORTWINDOW_H
