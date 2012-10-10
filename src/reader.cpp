#include "reader.h"

#include <QTextStream>

Reader::Reader(QObject *parent, QString fileName) :
    QObject(parent), file(NULL)
{
    openFile(fileName);
    if(isValid()) {
        readFile();
        file->close();
    }
}

void Reader::openFile(QString fileName) {
    file = new QFile(fileName, this);
    if( ! file->open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QFile::FileError errcode = file->error();
        error = tr("Cannot open file '%1', error code %2").arg(fileName).arg(errcode);
    }
}

void Reader::readFile() {
    QTextStream in(file);
    QString line;
    int lineNumber = 1;
    while( ! (line = in.readLine().trimmed()).isNull() ) {
        bool ok;
        double value = line.toDouble(&ok);
        if(ok) {
            parsedData << value;
        } else {
            warnings << tr("Line %1: failed to convert '%2' to number").arg(lineNumber).arg(line);
        }
        ++lineNumber;
    }
    if(parsedData.count() == 0) {
        error = tr("File contains no valid numerical data");
    }
}
