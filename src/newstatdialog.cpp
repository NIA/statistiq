#include "newstatdialog.h"
#include "ui_newstatdialog.h"
#include "reader.h"
#include <QDoubleValidator>
#include <QEvent>
#include <QMessageBox>
#include <QTextStream>

namespace {
    const int MAX_MESSAGES_COUNT = 5;
    QString skippedValuesMessage(const Reader::Parser & p) {
        int messagesCount = qMin(p.skippedLines.count(), MAX_MESSAGES_COUNT);
        QString result;
        for(int i = 0; i < messagesCount; ++i) {
            result += "\n";
            result += NewStatDialog::tr("'%1' at line %2").arg(p.skippedStrings[i]).arg(p.skippedLines[i]);
        }
        if(p.skippedLines.count() > messagesCount) {
            result += "\n...";
        }
        return result + "\n\n";
    }
}

NewStatDialog::NewStatDialog(QWidget *parent) :
    super(parent),
    ui(new Ui::NewStatDialog)
{
    ui->setupUi(this);
    ui->editInitValue->setValidator(new QDoubleValidator);

    ui->spinItemsNum->installEventFilter(this);
    ui->editInitValue->installEventFilter(this);
    ui->editValluesAsText->installEventFilter(this);
}

bool NewStatDialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if (object == ui->spinItemsNum  || object == ui->editInitValue) {
            ui->btnFillWithSame->setChecked(true);
        } else if (object == ui->editValluesAsText) {
            ui->btnFromText->setChecked(true);
        }
    }
    return false;
}

void NewStatDialog::accept() {
    if(ui->btnFillWithSame->isChecked()) {
        bool ok;
        double initValue = ui->editInitValue->text().toDouble(&ok);

        if( ! ok ) {
            QMessageBox::warning(this, tr("Invalid data"),
                                 tr("You have entered an invalid number '%1' as initial value")
                                 .arg(ui->editInitValue->text()));
            ui->editInitValue->setFocus();
            return;
        }

        data_ = QVector<double>(ui->spinItemsNum->value(), initValue).toList();
        super::accept();
    } else {
        QString text = ui->editValluesAsText->toPlainText();
        QTextStream in(&text, QIODevice::ReadOnly);
        Reader::Parser parser(in);
        if( ! parser.skippedLines.isEmpty() ) {
            QString message = tr("The following entered values are not correct numbers:");
            message += skippedValuesMessage(parser);
            message += tr("Would you like to skip these values?\n\nClick Yes to create a new Statistic with remaining values\nClick No to get back to editing and fix errors");
            int res = QMessageBox::question(this, tr("Skip incorrect values?"), message, QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No), QMessageBox::Yes );
            if(res == QMessageBox::No) {
                ui->editValluesAsText->setFocus();
                return;
            }
        }

        data_ = parser.parsedData;
        if(data_.isEmpty()) {
            QMessageBox::warning(this, tr("No valid data"), tr("You have not entered any valid data"));
            ui->editValluesAsText->setFocus();
            return;
        } else {
            super::accept();
        }
    }
}

NewStatDialog::~NewStatDialog()
{
    delete ui;
}
