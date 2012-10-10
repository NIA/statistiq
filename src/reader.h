#ifndef READER_H
#define READER_H

#include <QObject>
#include <QFile>
#include <QVector>

class Reader : public QObject
{
    Q_OBJECT
public:
    // Tries to open the file, retrieve data from it and
    // parse it into the vector of double. If something went
    // wrong, isValid() will return false. Otherwise data can
    // be retrieved via data() method.
    explicit Reader(QObject *parent, QString fileName);
    QList<double> data() { return parsedData; }

    QString errorMessage() { return error; }
    QList<QString> warningMessages() { return warnings; }
    bool isValid() { return error.isEmpty(); }

    QString formatReport();

private:
    QFile * file;
    QString error;
    QList<QString> warnings;
    QList<double> parsedData;

    // Initializing steps:
    void openFile(QString fileName);
    void readFile();
};

#endif // READER_H
