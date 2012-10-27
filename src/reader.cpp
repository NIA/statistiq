#include "reader.h"

#include <QFileInfo>

Reader::Parser::Parser(QTextStream &in)
{
    QString line;
    int lineNumber = 1;
    while( ! (line = in.readLine().trimmed()).isNull() ) {
        if(line.isEmpty()) { continue; }

        bool ok;
        double value = line.toDouble(&ok);
        if(ok) {
            parsedData << value;
        } else {
            skippedLines << lineNumber;
            skippedStrings << line;
        }
        ++lineNumber;
    }
}

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
    } else {
        QFileInfo fi(fileName);
        this->fileName = fi.fileName();
        this->lastModified = fi.lastModified();
    }
}

void Reader::readFile() {
    QTextStream in(file);
    Parser parser(in);
    for(int i = 0; i < parser.skippedLines.count(); ++i) {
        warnings << tr("Line %1: failed to convert '%2' to number")
                    .arg(parser.skippedLines[i])
                    .arg(parser.skippedStrings[i]);
    }
    if(parser.parsedData.count() == 0) {
        error = tr("File contains no valid numerical data");
    } else {
        parsedData = parser.parsedData;
    }
}

QString Reader::formatReport() {
    if(isValid()) {
        return tr("Loaded %1 values from %2, %3 skipped")
               .arg(parsedData.count())
               .arg(fileName)
               .arg(warnings.count());
    } else {
        return error;
    }
}

QString Reader::formatFileInfo() {
    if(isValid()) {
        return tr("%1 at %2").arg(fileName).arg(lastModified.toString(Qt::DefaultLocaleShortDate));
    } else {
        return tr("No file loaded");
    }
}

