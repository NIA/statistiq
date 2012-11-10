/*
 * (C) Copyright 2012 Kuban State University (http://kubsu.ru/),
 *     Faculty of Physics and Technology, Physics and Information Systems Chair
 *
 * All rights reserved. This file is part of ProcessData, a program
 * for getting various statistical information from measurement data.
 *
 * Developed by Ivan Novikov (http://github.com/NIA)
 * under the direction of Leontiy Grigorjan
 *
 */

#include "reader.h"
#include "logger.h"
#include "timer.h"

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
        Logger::error(error);
    } else {
        QFileInfo fi(fileName);
        this->fileName = fi.fileName();
        this->lastModified = fi.lastModified();
        Logger::trace(tr("Successfully opened file '%1'").arg(fileName));
    }
}

void Reader::readFile() {
    Timer timer;
    QTextStream in(file);
    Parser parser(in);
    for(int i = 0; i < parser.skippedLines.count(); ++i) {
        warnings << tr("Line %1: failed to convert '%2' to number, skipped")
                    .arg(parser.skippedLines[i])
                    .arg(parser.skippedStrings[i]);
        Logger::warning(warnings.last());
    }
    Logger::trace(tr("Read and parsed %1 values from file in %2 msecs").arg(parser.parsedData.count()).arg(timer.msecs()));
    if(parser.parsedData.count() == 0) {
        error = tr("File contains no valid numerical data");
        Logger::error(error);
    } else {
        parsedData = parser.parsedData;
        Logger::info(formatReport());
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
        return formatFileInfo(fileName, lastModified);
    } else {
        return tr("No file loaded");
    }
}

QString Reader::formatFileInfo(QString fileName, QDateTime lastModified) {
    return tr("%1 at %2").arg(fileName).arg(lastModified.toString(Qt::DefaultLocaleShortDate));
}

QString Reader::toShortFileName(QString filePath) {
    QFileInfo fi(filePath);
    return fi.fileName();
}

Writer::Writer(QObject *parent, QString fileName)
    : QObject(parent), file(NULL)
{
    file = new QFile(fileName, this);
    if( ! file->open(QIODevice::WriteOnly | QIODevice::Text )) {
        QFile::FileError errcode = file->error();
        error = tr("Cannot write to file '%1', error code %2").arg(fileName).arg(errcode);
        Logger::error(error);
    } else {
        Logger::trace(tr("Successfully opened file '%1' for writing").arg(fileName));
    }
}

void Writer::write(QList<double> data) {
    if( ! isValid() ) { return; }

    QTextStream out(file);
    foreach(double value, data) {
        // TODO: reuse valueToItem
        out << QString::number(value, 'g', 10) << endl;
    }
    Logger::info(tr("Successfully wrote %1 values to '%2'").arg(data.size()).arg(file->fileName()));
}

void Writer::close() {
    Logger::trace(tr("File '%1' closed").arg(file->fileName()));
    file->close();
    file = NULL;
}

Writer::~Writer() {
    close();
}
