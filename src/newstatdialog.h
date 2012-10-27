#ifndef NEWSTATDIALOG_H
#define NEWSTATDIALOG_H

#include <QDialog>

namespace Ui {
class NewStatDialog;
}

class NewStatDialog : public QDialog
{
    Q_OBJECT
    typedef QDialog super;
    
public:
    explicit NewStatDialog(QWidget *parent = 0);

    QList<double> data() { return data_; }

    ~NewStatDialog();

public slots:
    void accept();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::NewStatDialog *ui;

    QList<double> data_;
};

#endif // NEWSTATDIALOG_H
